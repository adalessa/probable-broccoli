#include "App.h"
#include "Color.h"
#include "HttpServer.h"
#include "Input.h"
#include "LcdApi.h"
#include "Rotation.h"
#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>
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

void TaskUI(void *pvParameters) {
  for (;;) {
    app->update(input);
    delay(10);
  }
}

void TaskHttpServer(void *pvParameters) {
  // Wait until WiFi is connected (in case UI task starts first)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  httpServer->begin();
  for (;;) {
    httpServer->handleLoop();
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

void setup() {
  Serial.begin(115200);
  lcd.initialize(Rotation::ROTATION_0, Color::Black);
  app = new App(lcd);

  httpServer = new HttpServerTask(*app);

  setupWifi();

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(-100000, 100000, false);
  input.setEncoder(&rotaryEncoder);

  app->changeMode(AppMode::Message);

  // Start UI task on Core 1
  xTaskCreatePinnedToCore(TaskUI, "TaskUI", 8192, NULL, 1, NULL, 1);

  // Start HTTP server task on Core 0
  xTaskCreatePinnedToCore(TaskHttpServer, "TaskHttpServer", 8192, NULL, 1, NULL,
                          0);
}
void loop() {}
