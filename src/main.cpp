#include "App.h"
#include "Color.h"
#include "HttpServer.h"
#include "Input.h"
#include "LcdApi.h"
#include "Rect.h"
#include "Rotation.h"
#include "Text.h"
#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WiFi.h>

// ========== ENCODER CONFIG ==========
#define ROTARY_ENCODER_A_PIN 33
#define ROTARY_ENCODER_B_PIN 32
#define ROTARY_ENCODER_BUTTON_PIN 35
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

// ========== GLOBAL OBJECTS ==========
LcdApi lcd;
App *app;
Input input;
HttpServerTask *httpServer;
WebSocketsClient webSocket;

void TaskUI(void *pvParameters) {
  for (;;) {
    app->update(input);
    delay(10);
  }
}
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {

  switch (type) {
  case WStype_DISCONNECTED:
    Serial.println("[WSc] Disconnected!\n");

    lcd.draw(Text("Dis")
                 .displayWidth(3)
                 .background(Color::DarkGreen)
                 .color(Color::White)
                 .position(LCD_WIDTH - (5 * 12), 0));
    break;
  case WStype_CONNECTED:
    Serial.printf("[WSc] Connected to url: %s\n", payload);

    // send message to server when Connected
    webSocket.sendTXT("Connected");

    lcd.draw(Text("Con")
                 .displayWidth(3)
                 .background(Color::DarkGreen)
                 .color(Color::White)
                 .position(LCD_WIDTH - (5 * 12), 0));
    break;
  case WStype_TEXT: {
    Serial.printf("[WSc] get text: %s\n", payload);

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (!err) {
      if (doc["action"] == "info") {
        JsonObject data = doc["data"];
        String player_status = data["player_status"];
        lcd.draw(Text(player_status.c_str())
                     .displayWidth(10)
                     .background(Color::DarkGreen)
                     .color(Color::White)
                     .position(0, 0));
      }

      if (doc["action"] == "screen") {

      }
      // if (doc["action"] == "test") {
      //   String data = doc["data"];
      //   lcd.draw(Text(data.c_str())
      //                .displayWidth(10)
      //                .background(Color::DarkGreen)
      //                .color(Color::White)
      //                .position(0, 0));
      // }
    }

    // send message to server
    // webSocket.sendTXT("message here");
    break;
  }
  case WStype_BIN:
  // 	Serial.printf("[WSc] get binary length: %zu\n", length);
  // 	// hexdump(payload, length);
  //
  // 	// send data to server
  // 	// webSocket.sendBIN(payload, length);
  // 	break;
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

void TaskHttpServer(void *pvParameters) {
  // Wait until WiFi is connected (in case UI task starts first)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.draw(Rect().color(Color::DarkGreen).start(0, 0).end(LCD_WIDTH, 24));
  webSocket.begin("10.27.22.22", 1337, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  // webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  httpServer->begin();
  for (;;) {
    httpServer->handleLoop();
    webSocket.loop();
    delay(10);
  }
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
  if (!app)
    return; // app is global

  webSocket.sendTXT("{\"action\": \"keypad\", \"data\": {\"key\": " +
                    String((int)key) + "}}");

  // switch (key) {
  // case KEY_1:
  //   // app->changeMode(AppMode::Message);
  //   webSocket.sendTXT("{\"action\": \"key\", \"data\": {\"key\":
  //   \"KEY_1\"}}"); break;
  // case KEY_2:
  //   // app->changeMode(AppMode::Klipper);
  //   webSocket.sendTXT("{\"action\": \"key\", \"data\": {\"key\":
  //   \"KEY_2\"}}"); break;
  // // Add more cases as needed
  // default:
  //   break;
  // }
}

void setup() {
  Serial.begin(115200);
  // Keypad matrix pin setup
  input.initializeKeypad();
  input.setKeyCallback(onKeypadEvent);
  lcd.initialize(Rotation::ROTATION_0, Color::Black);
  app = new App(lcd);

  httpServer = new HttpServerTask(*app);

  setupWifi();

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(-100000, 100000, false);
  input.setEncoder(&rotaryEncoder);

  app->changeMode(AppMode::Klipper);

  // Start UI task on Core 1
  xTaskCreatePinnedToCore(TaskUI, "TaskUI", 8192, NULL, 1, NULL, 1);

  // Start HTTP server task on Core 0
  xTaskCreatePinnedToCore(TaskHttpServer, "TaskHttpServer", 8192, NULL, 1, NULL,
                          0);
}

void loop() {
  // Call the new keypad handler; prints only on new press
  input.updateKeypad();
  delay(50);
  // if (input.isKeyPressed()) {
  //   Serial.print("KEY_");
  //   Serial.println(input.getLastPressedKey());
  // }
  // delay(50); // Small debounce & spam protection
}
