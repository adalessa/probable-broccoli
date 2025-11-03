#include "Color.h"
#include "LcdApi.h"
#include "Line.h"
#include "Rect.h"
#include "Rotation.h"
#include "Text.h"
#include <Arduino.h>

// ========== GLOBAL OBJECTS ==========
LcdApi lcd;

void TaskUI(void *pvParameters) {
  for (;;) {
    delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  lcd.initialize(Rotation::ROTATION_0, Color::Purple);

  lcd.send(Text("Play").color(Color::Black).background(Color::White));
  lcd.send(Rect().mode(Fill).start(200, 100).end(250, 150).color(Color::DarkGray));
  lcd.send(Rect().mode(Fill).start(205, 105).end(245, 145).color(Color::Purple));
  lcd.send(Line().start(205, 105).end(245, 145).color(Color::Black));


  // Start UI task on Core 1
  xTaskCreatePinnedToCore(TaskUI, "TaskUI", 8192, NULL, 1, NULL, 1);
}
void loop() {}
