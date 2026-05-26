#include "DynamicScreen.h"
#include "Color.h"
#include "Font.h"
#include "Icon.h"
#include "Line.h"
#include "Point.h"
#include "Rect.h"
#include "Text.h"
#include <ArduinoJson.h>
#include <WString.h>

DynamicScreen::DynamicScreen(LcdApi &lcd, WebSocketsClient &webSocket,
                             JsonObject data)
    : lcd(lcd), webSocket(webSocket), name("dynamic"), lastPayload(2048) {
  lastPayload.set(data);
}

void DynamicScreen::init() {
  // Use lastPayload to rebuild the screen
  if (lastPayload["initial_widgets"].is<JsonArray>()) {
    drawWidgets(lastPayload["initial_widgets"].as<JsonArray>());
  }
  initialized = true;
  lastRefresh = millis();
}

void DynamicScreen::update() {
  if (refreshTimeMs == 0)
    return;
  unsigned long nowMs = millis();
  if (nowMs - lastRefresh >= refreshTimeMs) {
    lastRefresh = nowMs;
    webSocket.sendTXT("{\"action\": \"refresh\", \"data\": {\"screen\": \"" +
                      name + "\"}}");
  }
}

void DynamicScreen::updateData(JsonObject data) {
  // Called on websocket payload; data must include full root

  String curr_name = data["screen_name"].as<const char *>();
  if (curr_name != name) {
    processPayload(data, false);
  } else {
    processPayload(data, initialized);
  }
}

void DynamicScreen::processPayload(JsonObject root, bool isUpdate) {
  // Parse config fields
  if (root.containsKey("screen_name")) {
    name = root["screen_name"].as<const char *>();
  }
  if (root.containsKey("refresh_time")) {
    if (root["refresh_time"].isNull()) {
      refreshTimeMs = 0;
    } else {
      refreshTimeMs = root["refresh_time"].as<unsigned long>() * 1000UL;
    }
  }
  // Store latest payload for refreshes
  lastPayload.clear();
  lastPayload.set(root);

  if (!isUpdate) {
    if (root["initial_widgets"].is<JsonArray>()) {
      drawWidgets(root["initial_widgets"].as<JsonArray>());
    }
  }

  // Update: always replace previous widgets (clear, draw)
  if (root["update_widgets"].is<JsonArray>()) {
    drawWidgets(root["update_widgets"].as<JsonArray>());
  }
}

void DynamicScreen::drawWidgets(JsonArray widgets) {
  for (JsonObject obj : widgets) {
    if (!obj.containsKey("type"))
      continue;
    String type = obj["type"].as<String>();
    if (type == "Text") {
      Text t = Text(obj["text"] | "");
      Font font = Font::x12x24;
      if (obj.containsKey("fontSize")) {
        int fontSize = obj["fontSize"].as<int>();
        switch (fontSize) {
        case 40:
          font = Font::x20x40;
          break;
        case 24:
          font = Font::x12x24;
          break;
        default:
          font = Font::x12x24;
          break;
        }
      }
      t.font(font);
      t.position(obj["x"] | 0, obj["y"] | 0);

      if (obj.containsKey("color")) {
        t.color(static_cast<Color>((uint16_t)obj["color"].as<uint16_t>()));
      }
      if (obj.containsKey("bg")) {
        t.background(static_cast<Color>((uint16_t)obj["bg"].as<uint16_t>()));
      }

      if (obj.containsKey("width")) {
        t.displayWidth(obj["width"].as<uint16_t>());
      }
      lcd.draw(t);
    } else if (type == "Rect") {
      uint16_t x = obj["x"] | 0;
      uint16_t y = obj["y"] | 0;
      uint16_t w = obj["width"] | 0;
      uint16_t h = obj["height"] | 0;
      Color color = Color::White;
      if (obj.containsKey("color")) {
        color = static_cast<Color>((uint16_t)obj["color"].as<uint16_t>());
      }
      lcd.draw(Rect().color(color).start(x, y).end(x + w, y + h));
    } else if (type == "Line") {
      uint16_t x1 = obj["x1"] | 0;
      uint16_t y1 = obj["y1"] | 0;
      uint16_t x2 = obj["x2"] | 0;
      uint16_t y2 = obj["y2"] | 0;
      Color color = Color::White;
      if (obj.containsKey("color")) {
        color = static_cast<Color>((uint16_t)obj["color"].as<uint16_t>());
      }
      lcd.draw(Line().color(color).start(x1, y1).end(x2, y2));
    } else if (type == "Point") {
      uint16_t x = obj["x"] | 0;
      uint16_t y = obj["y"] | 0;
      Color color = Color::White;
      if (obj.containsKey("color")) {
        color = static_cast<Color>((uint16_t)obj["color"].as<uint16_t>());
      }
      lcd.draw(Point().position(x, y).color(color));
    } else if (type == "Icon") {
      if (!obj.containsKey("iconName"))
        continue;
      const char *iconName = obj["iconName"];
      uint16_t x = obj["x"] | 0;
      uint16_t y = obj["y"] | 0;
      // TODO: Properly map iconName to IcondId, fallback if unknown
      IcondId iconId = IcondId::LOGO;
      lcd.draw(Icon(iconId).position(x, y));
    }
  }
}
