#include "KlipperComponent.h"
#include "Icon.h"
#include "IcondId.h"
#include "Input.h"
#include "Rect.h"
#include "Text.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

KlipperComponent::KlipperComponent(LcdApi &lcd) : lcd(lcd) {}

void KlipperComponent::update(Input input) {
  if (needsUpdate_) {
    draw();
    needsUpdate_ = false;
  }

  fetchData();
}

void KlipperComponent::draw() {
  lcd.draw(Rect().color(Color::DarkGreen).start(0, 0).end(LCD_WIDTH, 24));
  lcd.draw(Text("Klipper").color(Color::Black).position(5, 0));

  // format numbers with 1 decimal place manually
  auto toFixed = [](float value, int decimals = 1) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%4.*f", decimals, value);
    return std::string(buf);
  };

  std::string printLine =
      toFixed(printProgress_ * 100, 0) + "% (" + printState_ + ")    ";

  lcd.draw(Text(printLine)
               .color(Color::White)
               .background(Color::Black)
               .position(0, 30));

  std::string bedLine = toFixed(bedTemp_) + "/" + toFixed(bedTarget_, 0);
  std::string extLine =
      toFixed(extruderTemp_) + "/" + toFixed(extruderTarget_, 0);

  lcd.draw(Icon(IcondId::HotendTemp).position(5, 63));
  lcd.draw(Text(extLine)
               .color(Color::White)
               .background(Color::Black)
               .position(35, 60));
  lcd.draw(Icon(IcondId::BedTemp).position(5, 93));
  lcd.draw(Text(bedLine)
               .color(Color::White)
               .background(Color::Black)
               .position(35, 90));
}

void KlipperComponent::fetchData() {
  if (millis() - lastUpdate_ < 2000)
    return;
  lastUpdate_ = millis();

  HTTPClient http;
  http.begin("http://10.27.22.60/printer/objects/"
             "query?heater_bed&extruder&print_stats&display_status");
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    JsonDocument doc;

    DeserializationError err = deserializeJson(doc, payload);
    if (!err && doc["result"].is<JsonObject>()) {
      JsonObject status = doc["result"]["status"];

      // ✅ Heater bed
      if (status["heater_bed"].is<JsonObject>()) {
        JsonObject bed = status["heater_bed"];
        bedTemp_ = bed["temperature"] | 0.0f;
        bedTarget_ = bed["target"] | 0.0f;
      }

      // ✅ Extruder
      if (status["extruder"].is<JsonObject>()) {
        JsonObject extruder = status["extruder"];
        extruderTemp_ = extruder["temperature"] | 0.0f;
        extruderTarget_ = extruder["target"] | 0.0f;
      }

      // ✅ Print stats
      if (status["print_stats"].is<JsonObject>()) {
        JsonObject stats = status["print_stats"];
        printState_ = stats["state"].as<const char *>();
      }
      if (status["display_status"].is<JsonObject>()) {
        JsonObject displayStatus = status["display_status"];
        printProgress_ = displayStatus["progress"].as<const float>();
      }

      needsUpdate_ = true;
    }
  } else {
    bedTemp_ = extruderTemp_ = 0;
    bedTarget_ = extruderTarget_ = 0;
    printProgress_ = 0;
    printState_ = "offline";
    needsUpdate_ = true;
  }

  http.end();
}
