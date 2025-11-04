#include "App.h"
#include "Color.h"
#include "Input.h"
#include "LcdApi.h"
#include "Rotation.h"
#include "TasksComponent.h"
#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>

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

// ========== GLOBAL OBJECTS ==========
LcdApi lcd;
App *app;
Input input;

void TaskUI(void *pvParameters) {
  for (;;) {
    app->update(input);
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  lcd.initialize(Rotation::ROTATION_0, Color::Black);
  app = new App(lcd);

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(-100000, 100000, false);
  input.setEncoder(&rotaryEncoder);


  app->setComponent(new TasksComponent(lcd, input));
  // Start UI task on Core 1
  xTaskCreatePinnedToCore(TaskUI, "TaskUI", 8192, NULL, 1, NULL, 1);
}
void loop() {}
