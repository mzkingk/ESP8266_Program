#include <Arduino.h>

#include <TimeLib.h>
#include <WiFiUdp.h>
#include <U8g2lib.h>

#include "global.h"
#include "oled.h"

void sendNTPpacket(IPAddress &address);

String getCurrentTime(int hours, int minutes, int seconds);
String getCurrentDay(int years, int months, int days);

int isNTPConnected = 0;

unsigned char xing[] U8X8_PROGMEM = {
    0x00, 0x00, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x80, 0x00, 0x88, 0x00,
    0xF8, 0x1F, 0x84, 0x00, 0x82, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x80, 0x00, 0xFE, 0x3F, 0x00, 0x00}; /*星*/
unsigned char liu[] U8X8_PROGMEM = {
    0x40, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x02, 0x20, 0x04, 0x10, 0x08, 0x10, 0x10, 0x08, 0x10, 0x04, 0x20, 0x02, 0x20, 0x00, 0x00}; /*六*/

void initdisplay()
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 14);
    u8g2.print("Waiting for WiFi");
    u8g2.setCursor(0, 30);
    u8g2.print("connection...");
    u8g2.setCursor(0, 47);
    u8g2.print(AP_NAME);
    u8g2.setCursor(0, 64);
    u8g2.print("192.168.4.1");
    u8g2.sendBuffer();
    Serial.println("OLED Ready");
}

void oledClockDisplay()
{
    int years, months, days, hours, minutes, seconds, weekdays;
    years = year();
    months = month();
    days = day();
    hours = hour();
    minutes = minute();
    seconds = second();
    weekdays = weekday();
    Serial.printf("%d/%d/%d %d:%d:%d Weekday:%d\n", years, months, days, hours, minutes, seconds, weekdays);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 14);
    if (isNTPConnected < 3)
        u8g2.print("Network OK.");
    else
    {
        isNTPConnected = 3;
        u8g2.print("Network Error!");
    }

    String currentTime = getCurrentTime(hours, minutes, seconds);
    String currentDay = getCurrentDay(years, months, days);

    u8g2.setFont(u8g2_font_logisoso24_tr);
    u8g2.setCursor(0, 44);
    u8g2.print(currentTime);
    u8g2.setCursor(0, 61);
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.print(currentDay);
    u8g2.drawXBM(80, 48, 16, 16, xing);
    u8g2.setCursor(95, 62);
    u8g2.print("期");
    if (weekdays == 1)
        u8g2.print("日");
    else if (weekdays == 2)
        u8g2.print("一");
    else if (weekdays == 3)
        u8g2.print("二");
    else if (weekdays == 4)
        u8g2.print("三");
    else if (weekdays == 5)
        u8g2.print("四");
    else if (weekdays == 6)
        u8g2.print("五");
    else if (weekdays == 7)
        u8g2.drawXBM(111, 49, 16, 16, liu);
    u8g2.sendBuffer();
}

/*-------- NTP 代码 ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP时间在消息的前48个字节里
byte packetBuffer[NTP_PACKET_SIZE]; // 输入输出包的缓冲区
int timeZone = 8;

time_t getNtpTime()
{
    IPAddress ntpServerIP; // NTP服务器的地址

    while (Udp.parsePacket() > 0)
        continue; // 丢弃以前接收的任何数据包
    Serial.println("Transmit NTP Request");
    // 从池中获取随机服务器
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            isNTPConnected = 0;
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // 将数据包读取到缓冲区
            unsigned long secsSince1900;
            // 将从位置40开始的四个字节转换为长整型，只取前32位整数部分
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            Serial.println(secsSince1900);
            Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-("); //无NTP响应
    isNTPConnected++;
    return 0; //如果未得到时间则返回0
}

// 向给定地址的时间服务器发送NTP请求
void sendNTPpacket(IPAddress &address)
{
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    Udp.beginPacket(address, 123); // NTP需要使用的UDP端口号为123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

String getCurrentTime(int hours, int minutes, int seconds)
{
    String currentTime = "";
    if (hours < 10)
        currentTime += 0;
    currentTime += hours;
    currentTime += ":";
    if (minutes < 10)
        currentTime += 0;
    currentTime += minutes;
    currentTime += ":";
    if (seconds < 10)
        currentTime += 0;
    currentTime += seconds;
    return currentTime;
}

String getCurrentDay(int years, int months, int days)
{
    String currentDay = "";
    currentDay += years;
    currentDay += "/";
    if (months < 10)
        currentDay += 0;
    currentDay += months;
    currentDay += "/";
    if (days < 10)
        currentDay += 0;
    currentDay += days;
    return currentDay;
}