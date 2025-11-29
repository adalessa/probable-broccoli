#include "HttpServer.h"
#include "AppMode.h"
#include <Arduino.h>
#include <ArduinoJson.h>

HttpServerTask::HttpServerTask(App &app) : server(80), app(app) {}

void HttpServerTask::begin() {
  registerEndpoints();
  server.begin();
  Serial.println("HTTP server started");
}

void HttpServerTask::handleLoop() { server.handleClient(); }

void HttpServerTask::registerEndpoints() {
  // server.on("/counter", HTTP_GET, [this]() {
  //     std::lock_guard<std::mutex> lock(appMutex);
  //     String json = "{\"mode\":" + String(appState.mode) +
  //                   ",\"active\":" + String(appState.active ? "true" :
  //                   "false") + "}";
  //     server.send(200, "application/json", json);
  // });

  // server.on("/set", HTTP_POST, [this]() {
  //   String body = server.arg("plain");
  //   JsonDocument doc;
  //   DeserializationError error = deserializeJson(doc, body);
  //   if (error) {
  //     server.send(400, "application/json", "{\"error\":\"invalid json\"}");
  //     return;
  //   }
  //   if (!doc["value"]) {
  //     server.send(400, "application/json", "{\"error\":\"value missing\"}");
  //     return;
  //   }
  //   int value = doc["value"].as<int>();
  //   state.setCounter(value);
  //   server.send(200, "application/json", "{\"status\":\"ok\"}");
  // });

  server.on("/mode", HTTP_POST, [this]() {
    String body = server.arg("plain");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
      server.send(400, "application/json", "{\"error\":\"invalid json\"}");
      return;
    }
    if (doc["mode"] == nullptr) {
      server.send(400, "application/json", "{\"error\":\"mode missing\"}");
      return;
    }
    int mode = doc["mode"].as<int>();
    AppMode appMode = static_cast<AppMode>(mode);
    app.changeMode(appMode);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.on("/mode", HTTP_GET, [this]() {
    server.send(200, "application/json", "{\"mode\":" + String(static_cast<int>(app.getMode())) + "}");
  });

  server.on("/message", HTTP_POST, [this]() {
    String body = server.arg("plain");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
      server.send(400, "application/json", "{\"error\":\"invalid json\"}");
      return;
    }
    if (doc["message"] == nullptr) {
      server.send(400, "application/json", "{\"error\":\"message missing\"}");
      return;
    }
    std::string message = doc["message"].as<const char*>();
    app.getState().setMessage(message);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.on("/message", HTTP_GET, [this]() {
    std::string msg = app.getState().getMessage();
    server.send(200, "application/json", "{\"message\":\"" + String(msg.c_str()) + "\"}");
  });
}
