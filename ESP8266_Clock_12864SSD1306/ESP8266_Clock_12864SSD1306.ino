#include <Arduino.h>
/* 作者：flyAkari 会飞的阿卡林 bilibili UID:751219
 * 本代码适用于ESP8266 NodeMCU + 12864显示屏
 * 7pin SPI引脚，正面看，从左到右依次为GND、VCC、D0、D1、RES、DC、CS
 *    ESP8266 ---  OLED
 *      3V    ---  VCC
 *      G     ---  GND
 *      D7    ---  D1
 *      D5    ---  D0
 *      D2orD8---  CS
 *      D1    ---  DC
 *      RST   ---  RES
 * 4pin IIC引脚，正面看，从左到右依次为GND、VCC、SCL、SDA
 *      ESP8266  ---  OLED
 *      3.3V     ---  VCC
 *      G (GND)  ---  GND
 *      D1(GPIO5)---  SCL
 *      D2(GPIO4)---  SDA
 */
/**********************************************************************
 * 使用说明：
 * 初次上电后，用任意设备连接热点WiFi：mzking，等待登录页弹出或浏览器输入
 * 192.168.4.1进入WiFi及时钟配置页面，输入待连接WiFi名和密码、时区(-12~12)，
 * 填全后提交。若连接成功，则开发板会记住以上配置的信息，并在下次上电时自动连接
 * WiFi并显示时间，热点和配置页面不再出现。如需更改倒数日或WiFi信息，请关闭原
 * WiFi阻止其自动连接，上电后10秒无法登录则会重新开启热点和配置页面。
 ***********************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <U8g2lib.h>

#include "global.h"

void setup()
{
    ac.begin(); //开启空调红外
    Serial.begin(115200);
    while (!Serial)
        continue;

    Serial.println("NTP Clock oled version v1.1");
    initdisplay();

    WiFi.hostname("Smart-ESP8266");
    connectWiFi();

    Udp.begin(localPort);

    setSyncProvider(getNtpTime);
    setSyncInterval(30); //每300秒同步一次时间
}

time_t prevDisplay = 0; //当时钟已经显示

void loop()
{
    server.handleClient();
    dnsServer.processNextRequest();
    if (timeStatus() != timeNotSet)
    {
        if (now() != prevDisplay)
        { //时间改变时更新显示
            prevDisplay = now();
            oledClockDisplay();
        }
    }
    // bemfa模块使用
    doTCPClientTick();
}
