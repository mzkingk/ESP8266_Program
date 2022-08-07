#include <Arduino.h>

#include <IRremoteESP8266.h> // ESP8266红外控制库
#include <IRsend.h>          // 红外发送库
#include <ir_Coolix.h>       // 添加格力空调的库

#include "irServer.h"
#include "global.h"

bool isOn = false;
bool modeCool = false; // 模式，仅支持制冷和制热
bool fanAuto = false;  // 风速，仅支持自动和最小切换
int swingMode = 0;
uint8_t temp = 26;

// 空调开关
void handleIROnOff()
{
    Serial.println("successfully enter handle! on off");

    isOn = !isOn;
    ac.setPower(isOn);

    handleSendConfig();
}

// 温度+-
void handleTemperature()
{
    Serial.println("enter handle! temperature");

    temp = ac.getTemp() + (server.arg(0) == String('1') ? 1 : -1);
    ac.setTemp(temp);

    handleSendConfig();
}

// 设置模式，干燥/制冷等
void handleModeChange()
{
    Serial.println("enter handle! mode change");
    modeCool = !modeCool;
    ac.setMode(modeCool ? kCoolixCool : kCoolixHeat);

    handleSendConfig();
}
// 风速
void handleFanChange()
{
    Serial.println("enter handle! fan change");
    fanAuto = !fanAuto;
    ac.setFan(fanAuto ? kCoolixFanAuto : kCoolixFanMin);

    handleSendConfig();
}

// 切换摇摆开关
void swingAuto()
{
    Serial.println("enter handle! swing change");

    ac.setSwing();

    handleSendConfig();
}

void handleSendConfig()
{
    ac.send();

    Serial.println(ac.toString());

    String json = getJson();
    server.send(200, "text/plane", json);

    json = "";

    Serial.println("cmd is end");
}

String getJson()
{
    String json = "";
    json += "{";
    json += "\"status\":\"" + String(ac.getPower()) + "\",";
    json += "\"temperature\":\"" + String(ac.getTemp()) + "\",";
    json += "\"mode\":\"" + String(modeCool) + "\",";
    json += "\"modeDesc\":\"" + String(modeCool ? "cool" : "hot") + "\",";
    json += "\"fan\":\"" + String(fanAuto) + "\",";
    json += "\"fanDesc\":\"" + String(fanAuto ? "fan auto" : "fan min") + "\",";
    json += "\"swingAuto\":\"" + String(ac.getSwing()) + "\",";
    json += "\"swingAutoDesc\":\"" + String(ac.getSwing() ? "auto swing" : "off swing") + "\"";
    json += "}";
    Serial.print("json is:");
    Serial.println(json);
    return json;
}

void irActionHandler(String msg)
{
    Serial.printf(PSTR("msg: %s"), msg);
    if (msg.indexOf("#") <= 0)
    {
        if (msg.indexOf("on") >= 0)
        {
            isOn = true;
            ac.setPower(isOn);
            ac.send();
            Serial.println("its on.");
        }
        else if (msg.indexOf("off") >= 0)
        {
            isOn = false;
            ac.setPower(isOn);
            ac.send();
            Serial.println("its off.");
        }
        return;
    }

    char *p = (char *)msg.c_str();
    char *para = strsep(&p, "#");

    isOn = para[1] == 'n'; // on/off
    if (isOn)
    {
        Serial.print("start to change mode:");
        Serial.println(para[3]);
        modeCool = para[3] == '2';
        ac.setMode(modeCool ? kCoolixCool : kCoolixHeat);
        ac.send();
        getJson();

        char p2[5] = {};
        strncpy(p2, p + 2, 2); // 截取温度
        Serial.print("start change temp:");
        Serial.println(p2);

        temp = (uint8_t)strtol(p2, NULL, 10);
        ac.setTemp(temp);
        ac.send();
        getJson();
    }

    ac.setPower(isOn);
    ac.send();

    getJson();
    Serial.println("handler end");
}