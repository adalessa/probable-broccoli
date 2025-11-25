#include "HelloWorldComponent.h"
#include "Font.h"
#include "HTTPClient.h"
#include "Text.h"
#include <ArduinoJson.h>

HelloWorldComponent::HelloWorldComponent(LcdApi &lcd, AppState &state)
    : BaseComponent(lcd, state) {
  lcd.clear(Color::Blue);
}

void HelloWorldComponent::update(Input input) {
  if (needsDraw) {
    HTTPClient http;
    std::string url = "http://10.27.22.22:8000/api/info";
    http.begin(url.c_str());
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, payload);
      std::string msg = doc["msg"].as<const char *>();
      lcd.draw(Text(msg)
                   .font(Font::x32x64)
                   .color(Color::DarkGreen)
                   .background(Color::Black)
                   .position(10, 10));

      lcd.draw(Text(std::to_string(state.getCounter()))
                   .font(Font::x32x64)
                   .color(Color::DarkGreen)
                   .background(Color::Black)
                   .position(10, 100));
    } else {
      lcd.draw(Text("Fallo")
                   .font(Font::x32x64)
                   .color(Color::Red)
                   .background(Color::Black)
                   .position(10, 10));
    }

    needsDraw = false;
  }

  if (state.getCounter() != previousCounter) {
    needsDraw = true;
    previousCounter = state.getCounter();
  }
};
