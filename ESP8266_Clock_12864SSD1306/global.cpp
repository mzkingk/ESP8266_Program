#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <U8g2lib.h>

#include "global.h"

WiFiUDP Udp;
DNSServer dnsServer;
ESP8266WebServer server(80);

byte DNS_PORT = 53; // DNS端口号默认为53

IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

char sta_ssid[32] = {0};     //暂存WiFi名
char sta_password[64] = {0}; //暂存WiFi密码
char *AP_NAME = "mzking_display";    //自定义8266AP热点名
char *AP_PWD = "978964112";  //自定义热点密码，避免其他人也能连接该wifi

// 不同屏幕需要修改这里
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

char ntpServerName[] = "ntp1.aliyun.com"; // NTP服务器，阿里云

int localPort = 8888; // 用于侦听UDP数据包的本地端口

//********************需要修改的部分*******************//
String UID = "815bb03a60d5e4bc3373a57bfbf0e891"; //用户私钥，可在控制台获取,修改为自己的UID
String TOPIC = "ESP8266004";                   //主题名字，可在控制台新建
