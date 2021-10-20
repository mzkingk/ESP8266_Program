#include <Arduino.h>

#include "server.h"
#include "global.h"

void handleGetAll() {
    server.send(200, "text/plane", wiFiScan());

    Serial.println(F("successfully return a array"));
}

// wifi页面
void handleWifiPage() {
    server.send(200, "text/html", wifi_html);
}