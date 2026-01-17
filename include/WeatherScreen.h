#pragma once

#include "Font.h"
#include "IScreen.h"
#include "LcdApi.h"
#include "Rect.h"
#include "Text.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>

class WeatherScreen : public IScreen {
private:
  LcdApi &lcd;
  WebSocketsClient &webSocket;
  unsigned long lastRefresh = 0;

public:
  WeatherScreen(LcdApi &lcd, WebSocketsClient &webSocket)
      : lcd(lcd), webSocket(webSocket) {};

  String getName() override { return "weather"; }
  void init() override {
    // initial information
    lcd.draw(
        Rect().color(Color::Black).start(0, 24).end(LCD_WIDTH, LCD_HEIGHT));
    lcd.draw(Text("Clima")
                 .font(Font::x20x40)
                 .color(Color::White)
                 .position((LCD_WIDTH / 2) - (20 * 2.5), 30));

    lcd.draw(
        Text("Real:").font(Font::x20x40).color(Color::White).position(20, 80));

    lcd.draw(Text("Termica:")
                 .font(Font::x20x40)
                 .color(Color::White)
                 .position(20, 130));

    lcd.draw(Text("C")
                 .font(Font::x20x40)
                 .background(Color::Black)
                 .color(Color::DarkGray)
                 .position(260, 80));

    lcd.draw(Text("C")
                 .font(Font::x20x40)
                 .background(Color::Black)
                 .color(Color::DarkGray)
                 .position(260, 130));

    lcd.draw(Text("/")
                 .font(Font::x12x24)
                 .color(Color::DarkGray)
                 .background(Color::Black)
                 .position(50, 180));

    lcd.draw(Text("Hrs de luz")
                 .font(Font::x12x24)
                 .color(Color::DarkGray)
                 .background(Color::Black)
                 .position(20 + (4 * 12), 210));
  };
  void updateData(JsonObject data) override {

    lcd.draw(Text(data["current_condition"][0]["temp_C"].as<const char *>())
                 .font(Font::x20x40)
                 .background(Color::Black)
                 .color(Color::White)
                 .position(200, 80));

    lcd.draw(Text(data["current_condition"][0]["FeelsLikeC"].as<const char *>())
                 .font(Font::x20x40)
                 .background(Color::Black)
                 .color(Color::White)
                 .position(200, 130));

    lcd.draw(Text(data["weather"][0]["mintempC"].as<const char *>())
                 .font(Font::x12x24)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(20, 180));

    lcd.draw(Text(data["weather"][0]["maxtempC"].as<const char *>())
                 .font(Font::x12x24)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(68, 180));

    lcd.draw(Text(data["weather"][0]["sunHour"].as<const char *>())
                 .font(Font::x12x24)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(20, 210));

    lcd.draw(Text(data["weather"][0]["astronomy"][0]["sunrise"].as<const char *>())
                 .font(Font::x12x24)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(360, 180));

    lcd.draw(Text(data["weather"][0]["astronomy"][0]["sunset"].as<const char *>())
                 .font(Font::x12x24)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(360, 210));

    lastRefresh = millis();
  }

  void update() override {
    unsigned long nowMs = millis();
    if (nowMs - lastRefresh >= 30000 || lastRefresh == 0) {
      lastRefresh = nowMs;
      webSocket.sendTXT(
          "{\"action\": \"refresh\", \"data\": {\"screen\": \"weather\"}}");
    }
  };
};
