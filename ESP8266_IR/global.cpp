#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ir_Gree.h> //添加格力空调的库

#include "global.h"

DNSServer dnsServer;
ESP8266WebServer server(80);

byte DNS_PORT = 53; // DNS端口号默认为53

IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

char sta_ssid[32] = {0};     //暂存WiFi名
char sta_password[64] = {0}; //暂存WiFi密码
char *AP_NAME = "mzking";    //自定义8266AP热点名
char *AP_PWD = "978964112";  //自定义热点密码，避免其他人也能连接该wifi

int localPort = 8888; // 用于侦听UDP数据包的本地端口

uint16_t kIrLed = 14; // GPIO14
IRGreeAC ac(kIrLed);  //建立一个格力空调的控制实例

//********************需要修改的部分*******************//
String UID = "fds5f5sdf3ds4f3sd4f3ds3fsdf4sd3"; //用户私钥，可在控制台获取,修改为自己的UID
String TOPIC = "ESP8266IR005";                   //主题名字，可在控制台新建