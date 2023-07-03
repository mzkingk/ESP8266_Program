#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>

#include "global.h"

WiFiUDP Udp;
DNSServer dnsServer;
ESP8266WebServer server(80);

byte DNS_PORT = 53; // DNS端口号默认为53

IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

// 默认连接的WiFi名
char sta_ssid[32] = "";
// 默认连接的WiFi密码
char sta_password[64] = "";

// 自定义热点密码，避免其他人也能连接该wifi
char *AP_PWD = "esp-8266";

// 用于侦听UDP数据包的本地端口
int LOCAL_PORT = 8888;

// 端口
uint16_t LEDPIN = 0;

// 用户私钥，可在控制台获取,修改为自己的UID
String UID = "";

/*----------------- 以下是 每一个8366都要不重复的参数 --------------------*/

// 主题名字，可在巴法云控制台新建
String TOPIC = "myRoomLed002";
// 自定义8266AP热点名
char *AP_NAME = "esp-8266-my-room";
// 主机名
char *HOST_NAME = "esp-8266-my-room";