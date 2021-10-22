#include <Arduino.h>

#include <IRremoteESP8266.h> // ESP8266红外控制库
#include <IRsend.h>          // 红外发送库
#include <ir_Gree.h>         // 添加格力空调的库

#include "irServer.h"
#include "global.h"

int mode = 0;
int fan = 0;
int swingMode = 0;

void handleIROnOff()
{
    Serial.println("successfully enter handle! on off");

    ac.setPower(!ac.getPower());

    Serial.printf("flag is :");
    Serial.println(ac.getPower());

    ac.send();
    handleSendConfig();
}

void handleTemperature()
{
    Serial.println("enter handle! temperature");
    String arg = server.arg(0);

    const uint8_t temp = ac.getTemp() + (arg == String('1') ? 1 : -1);

    Serial.printf(PSTR("temperature is :%d, value is:%s\n"), temp, arg);

    ac.setTemp(temp);
    ac.send();
    handleSendConfig();
}

// kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
void handleModeChange()
{
    Serial.println("enter handle! mode change");
    mode++;
    mode = mode >= 5 ? 0 : mode;

    Serial.printf(PSTR("set mode end, mode is :%d\n"), mode);

    ac.setMode(mode);
    ac.send();
    handleSendConfig();
}

void handleFanChange()
{
    Serial.println("enter handle! fan change");
    fan++;
    fan = fan > 3 ? 0 : fan;

    ac.setFan(fan);

    Serial.printf(PSTR("set fan , is :%d\n"), ac.getFan());

    ac.send();
    handleSendConfig();
}

// 切换自动开关
void swingAuto()
{
    Serial.println("enter handle! swing change");

    bool automatic = !ac.getSwingVerticalAuto();

    ac.setSwingVertical(automatic, ac.getSwingVerticalPosition());

    Serial.printf(PSTR("set swing , is:"));
    Serial.println(automatic);
    Serial.println(ac.getSwingVerticalPosition());

    ac.send();
    handleSendConfig();
}

// 扫风模式
void swingChange()
{
    Serial.println("enter handle! swing change");

    bool automatic = ac.getSwingVerticalAuto();
    if (!automatic)
    {
        swingMode = swingMode + 1 > 4 ? 0 : swingMode + 1;
        const uint8_t m = swingMode == 0   ? kGreeSwingUp
                          : swingMode == 1 ? kGreeSwingMiddleUp
                          : swingMode == 2 ? kGreeSwingMiddle
                          : swingMode == 3 ? kGreeSwingMiddleDown
                                           : kGreeSwingDown;
        ac.setSwingVertical(automatic, m);
    }
    else
    {
        swingMode = swingMode + 1 > 3 ? 0 : swingMode + 1;
        const uint8_t m = swingMode == 0   ? kGreeSwingAuto
                          : swingMode == 1 ? kGreeSwingDownAuto
                          : swingMode == 2 ? kGreeSwingMiddleAuto
                                           : kGreeSwingUpAuto;
        ac.setSwingVertical(automatic, m);
    }

    Serial.printf(PSTR("set swing , is:\n"));
    Serial.println(automatic);
    Serial.println(ac.getSwingVerticalPosition());

    ac.send();
    handleSendConfig();
}

void handleSendConfig()
{
    Serial.println(ac.toString());
    String json = "";
    json += "{";
    json += "\"status\":\"" + String(ac.getPower()) + "\"";
    json += ",";
    json += "\"temperature\":\"" + String(ac.getTemp()) + "\"";
    json += ",";
    json += "\"mode\":\"" + String(mode) + "\"";
    json += ",";
    json += "\"fan\":\"" + String(fan) + "\"";
    json += ",";
    json += "\"swingAuto\":\"" + String(ac.getSwingVerticalAuto()) + "\"";
    json += ",";
    json += "\"swingMode\":\"" + String(ac.getSwingVerticalPosition()) + "\"";
    json += "}";
    server.send(200, "text/plane", json);

    Serial.printf("json is: ");
    Serial.println(json);
    json = "";

    Serial.println("cmd is end");
}