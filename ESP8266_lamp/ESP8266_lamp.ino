#include <Arduino.h>

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "global.h"

config_type config;

void setup()
{
    Serial.begin(115200);
    Serial.println("version v1.1");
    WiFi.hostname(HOST_NAME);

    loadConfig(); // 加载存储的数据
    apConfig();   // 加载ap

    Serial.println("config start");
    Serial.println(config.stassid);
    Serial.println(config.stapsw);
    Serial.println(config.cuid);
    Serial.println(config.ctopic);
    Serial.println(config.reboot);
    Serial.println(config.magic);
    Serial.println("config end");

    connectWiFi();

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDPIN, OUTPUT); // 初始化
    setSyncInterval(100);
}

void loop()
{
    dnsServer.processNextRequest();
    // bemfa模块使用
    do_client_tick();
}