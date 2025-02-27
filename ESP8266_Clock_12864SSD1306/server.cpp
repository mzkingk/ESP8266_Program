#include <Arduino.h>

#include "server.h"
#include "global.h"

// 启动WebServer
void initWebServer()
{
    Serial.println("enter init web!");

    server.on("/", HTTP_GET, handleIndexPage); //设置主页回调函数
    server.on("/wifi", HTTP_GET, handleWifiPage);

    server.onNotFound(handleIndexPage); //设置无法响应的http请求的回调函数

    server.on("/wifi", HTTP_POST, handleWifiPost); //设置Post请求回调函数
    server.on("/all", HTTP_GET, handleGetAll);     //查询所有wifi

    server.begin(); //启动WebServer

    Serial.println("WebServer started!");
    if (dnsServer.start(DNS_PORT, "*", apIP))
    {
        //判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dns server success.");
    }
    else
    {
        Serial.println("start dns server failed.");
    }
}

// 主页面
void handleIndexPage()
{
    Serial.println("successfully enter handle! index");
    server.send(200, "text/html", index_html);
    Serial.println("end handle! index");
}

// wifi页面
void handleWifiPage()
{
    Serial.println("successfully enter handle! wifi");
    server.send(200, "text/html", wifi_html);
}

// 查热点列表
void handleGetAll()
{
    Serial.println("successfully enter handle! getAll");
    server.send(200, "text/plane", wiFiScan());

    Serial.println(F("successfully return a array"));
}