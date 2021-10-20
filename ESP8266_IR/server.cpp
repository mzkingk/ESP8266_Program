#include <Arduino.h>

#include "server.h"
#include "global.h"

void handleGetAll() {
    server.send(200, "text/plane", wiFiScan());

    Serial.println(F("successfully return a array"));
}