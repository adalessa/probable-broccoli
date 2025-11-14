#include "KlipperComponent.h"
#include "Icon.h"
#include "IcondId.h"
#include "Input.h"
#include "Rect.h"
#include "Text.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

KlipperComponent::KlipperComponent(LcdApi &lcd, const std::vector<PrinterConfig> &printerConfigs) : lcd(lcd) {
  for (const auto &cfg : printerConfigs) {
    PrinterInfo info;
    info.ip = cfg.ip;
    info.name = cfg.name;
    printers.push_back(info);
  }
}

void KlipperComponent::update(Input input) {
  fetchData();
  bool anyNeedsUpdate = false;
  for (const auto& printer : printers) {
    if (printer.needsUpdate) {
      anyNeedsUpdate = true;
      break;
    }
  }
  if (anyNeedsUpdate) {
    draw();
    for (auto& printer : printers) {
      printer.needsUpdate = false;
    }
  }
}

void KlipperComponent::draw() {
  int printerCount = printers.size();
  int widthPerPrinter = LCD_WIDTH / printerCount;
  for (int i = 0; i < printerCount; ++i) {
    auto& printer = printers[i];
    if (!printer.needsUpdate) continue; // Only update if needed
    int xOffset = i * widthPerPrinter;
    // lcd.draw(Rect().color(Color::DarkGreen).start(xOffset, 0).end(xOffset + widthPerPrinter, 24));
    lcd.draw(Text(printer.name).color(Color::DarkGreen).position(xOffset + 5, 0));

    auto toFixed = [](float value, int decimals = 1) {
      char buf[16];
      snprintf(buf, sizeof(buf), "%4.*f", decimals, value);
      return std::string(buf);
    };

    std::string printLine =
        toFixed(printer.printProgress * 100, 0) + "% (" + printer.printState + ")    ";
    lcd.draw(Text(printLine)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(xOffset, 30));

    // Layer progression at the bottom
    std::string layerLine = "Layer: " + std::to_string(printer.currentLayer) + "/" + std::to_string(printer.totalLayer);
    lcd.draw(Text(layerLine)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(xOffset, 120));

    std::string bedLine = toFixed(printer.bedTemp) + "/" + toFixed(printer.bedTarget, 0);
    std::string extLine =
        toFixed(printer.extruderTemp) + "/" + toFixed(printer.extruderTarget, 0);

    lcd.draw(Icon(IcondId::HotendTemp).position(xOffset + 5, 63));
    lcd.draw(Text(extLine)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(xOffset + 35, 60));
    lcd.draw(Icon(IcondId::BedTemp).position(xOffset + 5, 93));
    lcd.draw(Text(bedLine)
                 .color(Color::White)
                 .background(Color::Black)
                 .position(xOffset + 35, 90));
    printer.needsUpdate = false;
  }
}

void KlipperComponent::fetchData() {
  for (auto &printer : printers) {
    if (millis() - printer.lastUpdate < 2000)
      continue;
    printer.lastUpdate = millis();

    HTTPClient http;
    std::string url = "http://" + printer.ip + "/printer/objects/query?heater_bed&extruder&print_stats&display_status";
    http.begin(url.c_str());
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err && doc["result"].is<JsonObject>()) {
        JsonObject status = doc["result"]["status"];
        if (status["heater_bed"].is<JsonObject>()) {
          JsonObject bed = status["heater_bed"];
          printer.bedTemp = bed["temperature"] | 0.0f;
          printer.bedTarget = bed["target"] | 0.0f;
        }
        if (status["extruder"].is<JsonObject>()) {
          JsonObject extruder = status["extruder"];
          printer.extruderTemp = extruder["temperature"] | 0.0f;
          printer.extruderTarget = extruder["target"] | 0.0f;
        }
        if (status["print_stats"].is<JsonObject>()) {
          JsonObject stats = status["print_stats"];
          printer.printState = stats["state"].as<const char *>();
          if (stats["info"].is<JsonObject>()) {
            JsonObject info = stats["info"];
            printer.currentLayer = info["current_layer"] | 0;
            printer.totalLayer = info["total_layer"] | 0;
          }
        }
        if (status["display_status"].is<JsonObject>()) {
          JsonObject displayStatus = status["display_status"];
          printer.printProgress = displayStatus["progress"].as<const float>();
        }
        printer.needsUpdate = true;
      }
    } else {
      printer.bedTemp = printer.extruderTemp = 0;
      printer.bedTarget = printer.extruderTarget = 0;
      printer.printProgress = 0;
      printer.printState = "offline";
      printer.needsUpdate = true;
    }
    http.end();
  }
}
