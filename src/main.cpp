#include "BeScheduleScreen.h"
#include "ClockScreen.h"
#include "Color.h"
#include "ConnectingScreen.h"
#include "EncoderAction.h"
#include "EncoderButtonAction.h"
#include "IScreen.h"
#include "Input.h"
#include "KeypadAction.h"
#include "LcdApi.h"
#include "Rotation.h"
#include "StatusBar.h"
#include "WeatherScreen.h"
#include "DynamicScreen.h"
#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WiFi.h>

// ========== ENCODER CONFIG ==========
#define ROTARY_ENCODER_A_PIN 33
#define ROTARY_ENCODER_B_PIN 32
#define ROTARY_ENCODER_BUTTON_PIN 22
#define ROTARY_ENCODER_VCC_PIN -1 // not used
#define ROTARY_ENCODER_STEPS 4

AiEsp32RotaryEncoder rotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN,
                                   ROTARY_ENCODER_BUTTON_PIN,
                                   ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS,
                                   false);

void IRAM_ATTR readEncoderISR() { rotaryEncoder.readEncoder_ISR(); }

// ========== WIFI CONFIG ==========
const char *ssid = "No Muggles Allowed";
const char *password = "98567928771545224286";
const char *websocket_server_ip = "10.27.22.22";
const int websocket_server_port = 1337;
const char *websocket_server_path = "/";

IScreen *screen;

// ========== GLOBAL OBJECTS ==========
LcdApi lcd;
Input input;
WebSocketsClient webSocket;
StatusBar statusBar(lcd);

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
  case WStype_DISCONNECTED:
    Serial.println("[WSc] Disconnected!\n");
    statusBar.setConnected(false);
    // screen = nullptr;

    // screen = new ClockScreen(lcd);
    // screen->init();
    break;
  case WStype_CONNECTED:
    Serial.printf("[WSc] Connected to url: %s\n", payload);

    // send message to server when Connected
    webSocket.sendTXT("Connected");

    statusBar.draw();
    statusBar.setConnected(true);
    break;
  case WStype_TEXT: {
    Serial.printf("[WSc] get text: %s\n", payload);
    Serial.printf("[WSc] Current name : %s\n", screen->getName().c_str());

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (!err) {
      if (doc["event"] == "info") {
        JsonObject data = doc["data"];
        String player_status = data["player_status"];
        statusBar.setPlayerStatus(player_status);
      }

      if (doc["event"] == "screen") {
        JsonObject data = doc["data"];
        String name = data["name"];
        bool force = data["force"] | false;
        bool isRefresh = false;
        if (screen) {
          isRefresh = name == screen->getName();
        }

        if (name == "weather") {
          if (!isRefresh || force) {
            screen = new WeatherScreen(lcd, webSocket);
            screen->init();
          }
          screen->updateData(data);
        }
        if (name == "dynamic") {
          if (!isRefresh || force) {
            screen = new DynamicScreen(lcd, webSocket, data);
            screen->init();
          }
          screen->updateData(data);
        }
        if (name == "be_schedule") {
          screen = new BeScheduleScreen(lcd);
          screen->init();
          screen->updateData(data);
        }
        if (name == "default") {
          screen = new ClockScreen(lcd);
          screen->init();
        }
      }
    }
    break;
  }
  case WStype_BIN:
  case WStype_ERROR:
  case WStype_PING:
  case WStype_PONG:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setupWebsocket() {
  webSocket.begin(websocket_server_ip, websocket_server_port,
                  websocket_server_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void onKeypadEvent(KeypadKey key) {
  String msg = KeypadAction(key).toString();
  webSocket.sendTXT(msg);
  Serial.println(msg);
}

void setup() {
  Serial.begin(115200);
  // Keypad matrix pin setup
  input.initializeKeypad();
  input.setKeyCallback(onKeypadEvent);
  lcd.initialize(Rotation::ROTATION_0, Color::Black);
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(-100, 100, false);

  // ConnectingScreen(lcd).init();

  screen = new ClockScreen(lcd);
  screen->init();

  setupWifi();
  setupWebsocket();

  // set up the time
  configTzTime("CET-1CEST,M3.5.0,M10.5.0", "pool.ntp.org");
}

void loop() {
  input.updateKeypad();
  webSocket.loop();

  if (rotaryEncoder.encoderChanged()) {
    int16_t encoderValue = rotaryEncoder.readEncoder();
    String msg = EncoderAction(encoderValue).toString();
    webSocket.sendTXT(msg);
    Serial.println(msg);
  }

  if (rotaryEncoder.isEncoderButtonClicked(100)) {
    String msg = EncoderButtonAction().toString();
    webSocket.sendTXT(msg);
    Serial.println(msg);
  }

  if (screen) {
    screen->update();
  }
  delay(50);
}
