#include <Arduino.h>

#include "server.h"
#include "global.h"

// 查热点列表
void handleGetAll()
{
    server.send(200, "text/plane", wiFiScan());

    Serial.println(F("successfully return a array"));
}

// wifi页面
void handleWifiPage()
{
    server.send(200, "text/html", wifi_html);
}

// 启动WebServer
void initWebServer()
{
    server.on("/", HTTP_GET, handleWifiPage);  //设置主页回调函数
    server.onNotFound(handleWifiPage);         //设置无法响应的http请求的回调函数
    server.on("/", HTTP_POST, handleWifiPost); //设置Post请求回调函数
    server.on("/all", HTTP_GET, handleGetAll); //查询所有wifi
    server.begin();                            //启动WebServer
    Serial.println("WebServer started!");
    if (dnsServer.start(DNS_PORT, "*", apIP))
    { //判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dns server success.");
    }
    else
    {
        Serial.println("start dns server failed.");
    }
}

// wifi页提交
void handleWifiPost()
{
    Serial.println("start to save wifi config");
    if (server.hasArg("ssid2"))
    {
        strcpy(sta_ssid, server.arg("ssid2").c_str());
    }
    else if (server.hasArg("ssid"))
    {
        strcpy(sta_ssid, server.arg("ssid").c_str());
    }
    else
    {
        Serial.println("[WebServer]Error, SSID not found!");
        server.send(200, "text/html", "<meta charset='UTF-8'>Error, SSID not found!"); //返回错误页面
        return;
    }
    Serial.printf(PSTR("ssid: %s\n"), sta_ssid);

    if (server.hasArg("password"))
    {
        strcpy(sta_password, server.arg("password").c_str());
        Serial.printf(PSTR("sta_password: %s\n"), sta_password);
    }
    else
    {
        Serial.println("[WebServer]Error, PASSWORD not found!");
        server.send(200, "text/html", "<meta charset='UTF-8'>Error, PASSWORD not found!");
        return;
    }
    server.send(200, "text/html", "<meta charset='UTF-8'>提交成功"); //返回保存成功页面
    delay(2000);
    //一切设定完成，连接wifi
    saveConfig();
    connectWiFi();
}

void saveConfig()
{ //存储配置到"EEPROM"
    Serial.println("save config");
    delay(1000);
}