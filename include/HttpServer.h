#pragma once

#include <WebServer.h>
#include "App.h"

class HttpServerTask {
public:
    HttpServerTask(App& app);
    void begin();       // setup endpoints and start server
    void handleLoop();  // handleClient loop

private:
    WebServer server;
    App& app;
    void registerEndpoints();
};

