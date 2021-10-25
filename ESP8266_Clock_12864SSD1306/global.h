#include <Arduino.h>

#ifndef _GLOBAL_H__
#define _GLOBAL_H__

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>

#include "html.h"
#include "mwifi.h"
#include "server.h"
#include "bemfa.h"
#include "sysConfig.h"
#include "oled.h"

extern WiFiUDP Udp;
extern DNSServer dnsServer;
extern ESP8266WebServer server;

extern byte DNS_PORT; // DNS端口号

extern IPAddress apIP; // 8266 APIP
extern IPAddress subnet;

extern char sta_ssid[32];     //暂存WiFi名
extern char sta_password[64]; //暂存WiFi密码
extern char *AP_NAME;         //自定义8266AP热点名
extern char *AP_PWD;          //自定义热点密码，避免其他人也能连接该wifi

// 不同屏幕需要修改这个
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern char ntpServerName[]; // NTP服务器，阿里云

extern int localPort; // 用于侦听UDP数据包的本地端口

extern String UID;   //用户私钥，可在控制台获取,修改为自己的UID
extern String TOPIC; //主题名字，可在控制台新建

#endif
