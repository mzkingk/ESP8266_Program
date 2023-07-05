#include <Arduino.h>

#ifndef _GLOBAL_H__
#define _GLOBAL_H__

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>

#include "mwifi.h"
#include "bemfa.h"

extern WiFiUDP Udp;
extern DNSServer dnsServer;
extern ESP8266WebServer server;

extern byte DNS_PORT; // DNS端口号

extern IPAddress apIP; // 8266 APIP
extern IPAddress subnet;

extern char sta_ssid[32];     // 暂存WiFi名
extern char sta_password[64]; // 暂存WiFi密码
extern int RETRY_COUNT;       // wifi重试次数

extern int LOCAL_PORT; // 用于侦听UDP数据包的本地端口
extern uint16_t LEDPIN;

extern String bemfa_type;
extern String bemfa_name;
extern String bemfa_proto;

extern String HOST_NAME; // 主机名

struct config_type
{
    char stassid[32];
    char stapsw[16];
    char cuid[40];
    char ctopic[32];
    uint8_t reboot;
    uint8_t magic;
};
extern config_type config;

#endif
