#include "HttpServer.h"
#include "AppState.h"
#include <Arduino.h>

HttpServerTask::HttpServerTask() : server(80) {}

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

  server.on("/set", HTTP_POST, [this]() {
    appState.counter = 2;
    server.send(200, "text/plain", "something");
  });
}
