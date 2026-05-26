#pragma once

#include "IScreen.h"
#include "LcdApi.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>

class DynamicScreen : public IScreen {
private:
  LcdApi &lcd;
  WebSocketsClient &webSocket;
  String name;
  unsigned long lastRefresh = 0;
  unsigned long refreshTimeMs = 0;
  bool initialized = false;

  // Store the last used payload for rebuild on refresh
  DynamicJsonDocument lastPayload;

public:
  DynamicScreen(LcdApi &lcd, WebSocketsClient &webSocket, JsonObject root);

  String getName() override { return "dynamic"; }
  void inital(JsonObject data);
  void init() override;
  void update() override;
  void updateData(JsonObject data) override;
  void drawWidgets(JsonArray widgets);
  void processPayload(JsonObject root, bool isUpdate);
};
