#pragma once

#include "IScreen.h"
#include "LcdApi.h"
#include "Rect.h"
#include "Text.h"
#include <WiFi.h>
#include <time.h>

class ClockScreen : public IScreen {
protected:
  LcdApi &lcd;
  unsigned long lastRefresh = 0;

public:
  ClockScreen(LcdApi &lcd)
      : lcd(lcd) {
  }
  String getName() override { return "default"; }
  void init() override {
    lcd.draw(
        Rect().color(Color::Black).start(0, 24).end(LCD_WIDTH, LCD_HEIGHT));
  }

  void updateData(JsonObject data) override {}

  void update() override {
    unsigned long nowMs = millis();
    if (nowMs - lastRefresh >= 1000 || lastRefresh == 0) {
      lastRefresh = nowMs;
      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        char timeStr[16];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

        lcd.draw(Text(timeStr)
                     .font(Font::x32x64)
                     .color(Color::White)
                     .background(Color::Black)
                     .position(110, LCD_HEIGHT / 2 - 32));
      } else {
        lcd.draw(Text("No time!")
                     .font(Font::x32x64)
                     .color(Color::Red)
                     .background(Color::Black)
                     .position(80, LCD_HEIGHT / 2 - 32));
      }
    }
  }
};
