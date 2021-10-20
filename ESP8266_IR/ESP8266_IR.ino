#include <Arduino.h>
/**********************************************************************
 * 使用说明：
 * 初次上电后，用任意设备连接热点WiFi：mzking，等待登录页弹出或浏览器输入
 * 192.168.4.1进入WiFi及时钟配置页面，输入待连接WiFi名和密码
 * 填全后提交。若连接成功，则开发板会记住以上配置的信息，并在下次上电时自动连接
 * WiFi并显示时间，热点和配置页面不再出现。如需更改倒数日或WiFi信息，请关闭原
 * WiFi阻止其自动连接，上电后10秒无法登录则会重新开启热点和配置页面。
 ***********************************************************************/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <EEPROM.h>

#include "global.h"

WiFiUDP Udp;
unsigned int localPort = 8888; // 用于侦听UDP数据包的本地端口

boolean isNTPConnected = false;

typedef struct
{           //存储配置结构体
    int tz; //时间戳
} config_type;
config_type config;

void saveConfig() { //存储配置到"EEPROM"
    Serial.println("save config");
    EEPROM.begin(sizeof(config));
    uint8_t* p = (uint8_t*) (&config);
    for (uint i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit(); //此操作会消耗flash写入次数
}

void loadConfig() { //从"EEPROM"加载配置
    Serial.println("load config");
    EEPROM.begin(sizeof(config));
    uint8_t* p = (uint8_t*) (&config);
    for (uint i = 0; i < sizeof(config); i++)
    {
        *(p + i) = EEPROM.read(i);
    }
    delay(2000);
}

char sta_ssid[32] = { 0 };        //暂存WiFi名
char sta_password[64] = { 0 };    //暂存WiFi密码
const char* AP_NAME = "mzking"; //自定义8266AP热点名
const char* AP_PWD = "978964112"; //自定义热点密码，避免其他人也能连接该wifi

const byte DNS_PORT = 53;       // DNS端口号默认为53
IPAddress apIP(192, 168, 4, 1); // 8266 APIP
IPAddress subnet(255, 255, 255, 0);

DNSServer dnsServer;
ESP8266WebServer server(80);

void connectWiFi();
void printDebugInfo();
void handleGetAll();
void initWebServer();

void handleRoot() {
    server.send(200, "text/html", wifi_html);
}
void handleRootPost() {
    Serial.println("handleRootPost");
    if (server.hasArg("ssid2")) {
        strcpy(sta_ssid, server.arg("ssid2").c_str());
    } else if (server.hasArg("ssid")) {
        strcpy(sta_ssid, server.arg("ssid").c_str());
    } else {
        Serial.println("[WebServer]Error, SSID not found!");
        server.send(200, "text/html", "<meta charset='UTF-8'>Error, SSID not found!"); //返回错误页面
        return;
    }
    Serial.printf(PSTR("ssid: %s\n"), sta_ssid);

    if (server.hasArg("password")) {
        strcpy(sta_password, server.arg("password").c_str());
        Serial.printf(PSTR("sta_password: %s\n"), sta_password);
    } else {
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

void handleGetAll() {
    server.send(200, "text/plane", wiFiScan());

    Serial.println(F("successfully return a array"));
}

void initWebServer() {
    server.on("/", HTTP_GET, handleRoot);      //设置主页回调函数
    server.onNotFound(handleRoot);             //设置无法响应的http请求的回调函数
    server.on("/", HTTP_POST, handleRootPost); //设置Post请求回调函数
    server.on("/all", HTTP_GET, handleGetAll);    //查询所有wifi
    server.begin();                            //启动WebServer
    Serial.println("WebServer started!");
    if (dnsServer.start(DNS_PORT, "*", apIP)) { //判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    } else {
        Serial.println("start dnsserver failed.");
    }
}

void connectWiFi() {
    WiFi.mode(WIFI_STA);       //切换为STA模式
    WiFi.setAutoConnect(true); //设置自动连接
    WiFi.begin(sta_ssid, sta_password);
    Serial.println("Connect WiFi");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        count++;
        if (count > 20) { // 10秒过去依然没有自动连上，开启Web配网功能，可视情况调整等待时长
            Serial.println("Timeout! AutoConnect failed");
            WiFi.mode(WIFI_AP); //开热点
            WiFi.softAPConfig(apIP, apIP, subnet);
            if (WiFi.softAP(AP_NAME, AP_PWD, 7)) {
                Serial.println("ESP8266 SoftAP is on");
            }
            initWebServer();// 启动WebServer

            Serial.println("Please connect the WiFi named mzking, the configuration page will pop up automatically, if not, use your browser to access 192.168.4.1");
            break; // 启动WebServer后便跳出while循环，回到loop
        }
        Serial.print(".");
        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.print("password:");
            Serial.print(WiFi.psk().c_str());
            Serial.println(" is incorrect");
        }
        if (WiFi.status() == WL_NO_SSID_AVAIL) {
            Serial.print("configured SSID:");
            Serial.print(WiFi.SSID().c_str());
            Serial.println(" cannot be reached");
        }
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        server.stop();
        dnsServer.stop();
        // WiFi连接成功后，热点便不再开启，无法再次通过web配网
        //若WiFi连接断开，ESP8266会自动尝试重新连接，直至连接成功，无需代码干预
        //如需要更换WiFi，请在关闭原WiFi后重启ESP8266，否则上电后会自动连接原WiFi，也就无法进入配网页面
    }
}

void setup() {
    initWifiHtml(wifi_html);

    Serial.begin(115200);
    while (!Serial)
        continue;
    Serial.println("Designed by mzking");

    Serial.print("load config...");
    loadConfig();

    WiFi.hostname("Smart-ESP8266");
    connectWiFi();

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.printf(PSTR("Local port: %d\n"), Udp.localPort());

    printDebugInfo();

    delay(2000);
}

void loop() {
    server.handleClient();
    dnsServer.processNextRequest();
    delay(2000);
}



void printDebugInfo() {
    Serial.printf(PSTR("WiFi page html is %s\n"), wifi_html);
}