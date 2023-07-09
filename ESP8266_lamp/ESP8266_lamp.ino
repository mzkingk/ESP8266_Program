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

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDPIN, OUTPUT); // 初始化

    String mac = WiFi.macAddress().substring(8); // 取mac地址做主题用
    mac.replace(":", "");                        // 去掉:号
    WiFi.hostname(mac);

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
    setSyncInterval(100);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }
    dnsServer.processNextRequest();
    // bemfa模块使用
    doClientTick();
}