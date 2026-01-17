#pragma once

#include "IScreen.h"
#include "LcdApi.h"
#include "Rect.h"
#include "Text.h"
#include <WiFi.h>

class BeScheduleScreen : public IScreen {
protected:
  LcdApi &lcd;
  // Configurable presentation parameters -- EDIT HERE if needed
  Font font = Font::x16x32;
  int startY = 40;
  int lineOffset = 40;
  int colDay = 10;    // X position for day
  int colStart = 200; // X position for start time
  int colDash = 300;  // X position for dash
  int colEnd = 330;   // X position for end time

public:
  BeScheduleScreen(LcdApi &lcd)
    : lcd(lcd) {}

  String getName() override { return "schedule"; }

  void init() override {
    lcd.draw(
      Rect().color(Color::Black).start(0, 24).end(LCD_WIDTH, LCD_HEIGHT));
  }

  void updateData(JsonObject data) override {
    if (data["schedule"].isNull())
      return;
    JsonArray scheduleArr = data["schedule"].as<JsonArray>();
    int y = startY;
    for (JsonObject entry : scheduleArr) {
      String day = entry["day"];
      String start = entry["start"];
      String end = entry["end"];
      // Draw day label
      lcd.draw(Text(std::string(day.c_str()))
                .font(font)
                .color(Color::White)
                .position(colDay, y));
      // Draw start time
      lcd.draw(Text(std::string(start.c_str()))
                .font(font)
                .color(Color::White)
                .position(colStart, y));
      // Draw dash
      lcd.draw(Text("-")
                .font(font)
                .color(Color::White)
                .position(colDash, y));
      // Draw end time
      lcd.draw(Text(std::string(end.c_str()))
                .font(font)
                .color(Color::White)
                .position(colEnd, y));
      y += lineOffset;
    }
  }

  void update() override {
    // intentionally left empty! all work is done in updateData
  }
};
