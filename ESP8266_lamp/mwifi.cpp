#include <Arduino.h>

#include "mwifi.h"
#include "global.h"

char config_flag = 0;   // 判断是否配网
char packetBuffer[255]; // 发送数据包
Ticker delayTimer;

// 从EEPROM加载参数
uint8_t *p = (uint8_t *)(&config);

void loadConfig()
{

    uint8_t mac[6];
    Serial.println("Load config.......");
    WiFi.macAddress(mac);
    EEPROM.begin(512);
    for (int i = 0; i < sizeof(config); i++)
    {
        *(p + i) = EEPROM.read(i);
    }
    config.reboot = config.reboot + 1;
    if (config.reboot >= 4)
    {
        restoreFactory();
    }
    if (config.magic != 0xAA)
    {
        config_flag = 1;
    }
    EEPROM.begin(512);
    for (int i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
    delay(2000);
    Serial.println("loadConfig Over");
    EEPROM.begin(512);
    config.reboot = 0;
    for (int i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
}

// 恢复出厂设置
void restoreFactory()
{
    Serial.println("\r\n Restore Factory....... ");
    config.magic = 0x00;
    strcpy(config.stassid, "");
    strcpy(config.stapsw, "");
    strcpy(config.cuid, "");
    strcpy(config.ctopic, "");
    config.magic = 0x00;
    saveConfig();
    delayRestart(1);
    while (1)
    {
        ESP.wdtFeed();
        delay(100);
    }
}

// 保存WIFI信息
void saveConfig()
{
    config.reboot = 0;
    EEPROM.begin(2018);
    uint8_t *p = (uint8_t *)(&config);
    for (int i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
}

void delayRestart(float t)
{
    delayTimer.attach(t, []()
                      { ESP.restart(); });
}
void apConfig()
{
    String mac = WiFi.macAddress().substring(8); // 取mac地址做主题用
    mac.replace(":", "");                        // 去掉:号
    if (config_flag == 1)
    {
        WiFi.softAP("bemfa_" + mac);
        Udp.begin(LOCAL_PORT);
        Serial.println("Started Ap Config...please connect with wechat");
    }
    String topic = mac + bemfa_type;
    while (config_flag)
    {
        // 如果未配网，开启AP配网，并接收配网信息
        int packetSize = Udp.parsePacket();
        if (packetSize)
        {
            Serial.print("Received packet from ");
            Serial.print(Udp.remoteIP());

            int len = Udp.read(packetBuffer, 255);
            if (len > 0)
            {
                packetBuffer[len] = 0;
            }
            Serial.println("\nContents:");
            Serial.println(packetBuffer);
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, packetBuffer);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }
            int cmdType = doc["cmdType"].as<int>();
            if (cmdType == 1)
            {
                const char *ssid = doc["ssid"];
                const char *password = doc["password"];
                const char *token = doc["token"];
                Serial.println(ssid);
                strcpy(config.stassid, ssid);
                strcpy(config.stapsw, password);
                strcpy(config.cuid, token);
                config.reboot = 0;
                config.magic = 0xAA;
                strcpy(config.ctopic, topic.c_str());
                saveConfig();
                // 收到信息，并回复
                String reply_buffer = "{\"cmdType\":2,\"productId\":\"" + topic + "\",\"deviceName\":\"" + bemfa_name + "\",\"protoVersion\":\"" + bemfa_proto + "\"}";
                Serial.println(reply_buffer);
                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                Udp.write(reply_buffer.c_str());
                Udp.endPacket();
            }
            else if (cmdType == 3)
            {
                config_flag = 0;
                WiFi.softAPdisconnect(true);
            }
        }
    }
}

void connectWiFi()
{
    WiFi.disconnect();         // 断开连接
    WiFi.mode(WIFI_STA);       // 切换为STA模式
    WiFi.setAutoConnect(true); // 设置自动连接
    WiFi.begin(config.stassid, config.stapsw);
    Serial.printf(PSTR("start to connect WiFi: %s\n"), config.stassid);
    digitalWrite(LED_BUILTIN, HIGH); // 状态灯，low亮灯，high熄灭
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        count++;
        if (count > RETRY_COUNT)
        {
            // n秒过去依然没有自动连上，开启Web配网功能，可视情况调整等待时长
            Serial.println("Timeout! AutoConnect failed");
            restoreFactory();
            break;
        }
        Serial.print(".");
        if (WiFi.status() == WL_CONNECT_FAILED)
        {
            Serial.print("password:");
            Serial.print(WiFi.psk().c_str());
            Serial.println(" is incorrect");
        }
        if (WiFi.status() == WL_NO_SSID_AVAIL)
        {
            Serial.print("configured SSID:");
            Serial.print(WiFi.SSID().c_str());
            Serial.println(" cannot be reached");
        }
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("WiFi Connected!\nIP address: ");
        Serial.println(WiFi.localIP());
        digitalWrite(LED_BUILTIN, HIGH); // 状态灯，low亮灯，high熄灭
    }
}