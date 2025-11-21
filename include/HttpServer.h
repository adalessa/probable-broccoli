#pragma once

#include <WebServer.h>

class HttpServerTask {
public:
    HttpServerTask();
    void begin();       // setup endpoints and start server
    void handleLoop();  // handleClient loop

private:
    WebServer server;

    void registerEndpoints();
};
