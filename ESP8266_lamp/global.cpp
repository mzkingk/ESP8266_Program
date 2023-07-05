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
// wifi重试次数
int RETRY_COUNT = 3600;
// 用于侦听UDP数据包的本地端口
int LOCAL_PORT = 8266;
// 端口
uint16_t LEDPIN = 0;

// 根据需要修改的信息
String bemfa_type = "002";    // 设备类型
String bemfa_name = "灯"; // 设备昵称，可随意修改
String bemfa_proto = "3";     // 3是tcp设备端口8344,1是MQTT设备

/*----------------- 以下是 每一个8366都不重复的参数 --------------------*/

// 主机名
String HOST_NAME = "esp-8266-my-room";