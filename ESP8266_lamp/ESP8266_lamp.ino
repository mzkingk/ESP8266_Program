#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <TimeLib.h>
#include <EEPROM.h>

#include "global.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("version v1.1");
    WiFi.hostname(HOST_NAME);
    connectWiFi();
    Udp.begin(LOCAL_PORT);
    pinMode(LEDPIN, OUTPUT); // 初始化
    setSyncInterval(60);
}

void loop()
{
    server.handleClient();
    dnsServer.processNextRequest();
    // bemfa模块使用
    doTCPClientTick();
}
