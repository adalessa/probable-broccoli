#pragma once

#include "Icon.h"
#include "LcdApi.h"
#include "Rect.h"
#include "Text.h"

class StatusBar {
private:
  LcdApi &lcd;
  bool connected = false;
  String playerStatus = "";
  Color bgColor = Color::DarkGray;

public:
  StatusBar(LcdApi &lcd) : lcd(lcd) {};
  void setConnected(bool isConnected) {
    connected = isConnected;
    update();
  };
  void setPlayerStatus(const String &status) {
    playerStatus = status;
    update();
  };
  void draw() {
    lcd.draw(Rect().color(bgColor).start(0, 0).end(LCD_WIDTH, 24));

    lcd.draw(
        Text("Player: ").displayWidth(10).color(Color::Gray).position(0, 0));
  };

  void update() {

    if (connected) {
      lcd.draw(Icon(IcondId::Motion).position(LCD_WIDTH - 22, 3));
    } else {
      lcd.draw(Icon(IcondId::ResumeEEPROM).position(LCD_WIDTH - 22, 3));
    }

    lcd.draw(Text(playerStatus.c_str())
                 .displayWidth(10)
                 .background(bgColor)
                 .color(Color::White)
                 .position(12 * 8, 0));
  }
};
