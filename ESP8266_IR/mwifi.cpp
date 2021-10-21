#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "mwifi.h"
#include "global.h"

String wiFiScan()
{
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t *bssid;
    int32_t channel;
    bool hidden;
    int scanResult;

    Serial.println(F("Starting WiFi scan..."));

    scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (scanResult == 0)
    {
        Serial.println(F("No networks found"));
    }
    else if (scanResult > 0)
    {
        Serial.printf(PSTR("%d networks found:\n"), scanResult);

        String list = String("[\"");
        for (int8_t i = 0; i < scanResult; i++)
        {
            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

            Serial.printf(PSTR("               %ddBm %s\n"), rssi, ssid.c_str());
            yield();

            if (ssid.length() > 0)
            {
                String str = String((int)rssi);
                list += str;
                list += "dBm ";
                list += ssid.c_str();
                list += "\",\"";
            }
        }

        if (list.length() > 2)
        {
            list.remove(list.length() - 2);
        }
        list += "]";

        return list;
    }
    else
    {
        Serial.printf(PSTR("WiFi scan error %d"), scanResult);
    }
    return "[]";
}

void connectWiFi()
{
    WiFi.mode(WIFI_STA);       //切换为STA模式
    WiFi.setAutoConnect(true); //设置自动连接
    WiFi.begin(sta_ssid, sta_password);
    Serial.println("Connect WiFi");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        count++;
        if (count > 20)
        { // 10秒过去依然没有自动连上，开启Web配网功能，可视情况调整等待时长
            Serial.println("Timeout! AutoConnect failed");
            WiFi.mode(WIFI_AP); //开热点
            WiFi.softAPConfig(apIP, apIP, subnet);
            if (WiFi.softAP(AP_NAME, AP_PWD, 7))
            {
                Serial.println("ESP8266 SoftAP is on");
            }
            initWebServer(); // 启动WebServer

            Serial.println("Please connect the WiFi named mzking, the configuration page will pop up automatically, if not, use your browser to access 192.168.4.1");
            break; // 启动WebServer后便跳出while循环，回到loop
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
        Serial.printf(PSTR("WiFi Connected!\nIP address: %s\n"), WiFi.localIP());

        // WiFi连接成功后，热点便不再开启，可以在局域网内通过ip进入配网页面操作
        //若WiFi连接断开，ESP8266会自动尝试重新连接，直至连接成功，无需代码干预
        //如需要更换WiFi，请在关闭原WiFi后重启ESP8266，否则上电后会自动连接原WiFi，也就无法进入配网页面
    }
}