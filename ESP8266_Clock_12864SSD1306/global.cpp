#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <U8g2lib.h>
#include <ir_Coolix.h> //添加空调的库

#include "global.h"

WiFiUDP Udp;
DNSServer dnsServer;
ESP8266WebServer server(80);

byte DNS_PORT = 53; // DNS端口号默认为53

IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

//默认连接的WiFi名
char sta_ssid[32] = "CMCC-3G6W";
//默认连接的WiFi密码
char sta_password[64] = "";
//自定义8266AP热点名
char *AP_NAME = "esp-8266";
//自定义热点密码，避免其他人也能连接该wifi
char *AP_PWD = "esp-8266";

// 不同屏幕需要修改这里
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// NTP服务器，阿里云
char ntpServerName[] = "ntp1.aliyun.com";

// 用于侦听UDP数据包的本地端口
int localPort = 8888;

// 红外发射管信号指针端口
uint16_t kIrLed = 14;
//建立一个空调的控制实例，不同空调这里需修改
IRCoolixAC ac(kIrLed);

//用户私钥，可在控制台获取,修改为自己的UID
String UID = "";
//主题名字，可在控制台新建
String TOPIC = "ESP8266IR005";