#include <Arduino.h>
/**********************************************************************
 * 使用说明：
 * 初次上电后，用任意设备连接热点WiFi：mzking，等待登录页弹出或浏览器输入
 * 192.168.4.1进入WiFi及时钟配置页面，输入待连接WiFi名和密码
 * 填全后提交。若连接成功，则开发板会记住以上配置的信息，并在下次上电时自动连接
 * WiFi并显示时间，热点和配置页面不再出现。如需更改倒数日或WiFi信息，请关闭原
 * WiFi阻止其自动连接，上电后10秒无法登录则会重新开启热点和配置页面。
 ***********************************************************************/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <SPI.h>
#include <EEPROM.h>

#include "global.h"

void printDebugInfo();

void setup()
{
    ac.begin(); //开启空调红外
    Serial.begin(115200);
    while (!Serial)
        continue;
    Serial.println("Designed by mzking");

    loadConfig();

    WiFi.hostname("Smart-ESP8266");
    connectWiFi();

    printDebugInfo();

    delay(2000);
}

void loop()
{
    server.handleClient();
    dnsServer.processNextRequest();

    // bemfa模块使用
    doTCPClientTick();
}

void printDebugInfo()
{
    Serial.println("WiFi page html ok.");
}