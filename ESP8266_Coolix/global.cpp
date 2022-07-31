#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ir_Coolix.h> //添加空调的库

#include "global.h"

DNSServer dnsServer;
ESP8266WebServer server(80);

byte DNS_PORT = 53; // DNS端口号默认为53

IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

int localPort = 8888; // 用于侦听UDP数据包的本地端口

//********************以下按需修改*******************//

//默认连接的WiFi名
char sta_ssid[32] = "CMCC-3G6W";
//默认连接的WiFi密码
char sta_password[64] = "";
//自定义8266AP热点名
char *AP_NAME = "mzking";
//自定义热点密码，避免其他人也能连接该wifi
char *AP_PWD = "";

// 信号指针端口，比如14代表的是GPIO14
uint16_t kIrLed = 14;
//建立一个空调的控制实例，不同空调这里需修改
IRCoolixAC ac(kIrLed);

//用户私钥，可在控制台获取,修改为自己的UID
String UID = "";
//主题名字，可在控制台新建
String TOPIC = "ESP8266IR005";