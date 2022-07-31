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

    geli.setPower(!geli.getPower());

    Serial.printf("flag is :");
    Serial.println(geli.getPower());

    geli.send();
    handleSendConfig();
}

void handleTemperature()
{
    Serial.println("enter handle! temperature");
    String arg = server.arg(0);

    const uint8_t temp = geli.getTemp() + (arg == String('1') ? 1 : -1);

    Serial.printf(PSTR("temperature is :%d, value is:%s\n"), temp, arg);

    geli.setTemp(temp);
    geli.send();
    handleSendConfig();
}

// kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
void handleModeChange()
{
    Serial.println("enter handle! mode change");
    mode++;
    mode = mode >= 5 ? 0 : mode;

    Serial.printf(PSTR("set mode end, mode is :%d\n"), mode);

    geli.setMode(mode);
    geli.send();
    handleSendConfig();
}

void handleFanChange()
{
    Serial.println("enter handle! fan change");
    fan++;
    fan = fan > 3 ? 0 : fan;

    geli.setFan(fan);

    Serial.printf(PSTR("set fan , is :%d\n"), geli.getFan());

    geli.send();
    handleSendConfig();
}

// 切换自动开关
void swingAuto()
{
    Serial.println("enter handle! swing change");

    bool automatic = !geli.getSwingVerticalAuto();

    geli.setSwingVertical(automatic, geli.getSwingVerticalPosition());

    Serial.printf(PSTR("set swing , is:"));
    Serial.println(automatic);
    Serial.println(geli.getSwingVerticalPosition());

    geli.send();
    handleSendConfig();
}

// 扫风模式
void swingChange()
{
    Serial.println("enter handle! swing change");

    bool automatic = geli.getSwingVerticalAuto();
    if (!automatic)
    {
        swingMode = swingMode + 1 > 4 ? 0 : swingMode + 1;
        const uint8_t m = swingMode == 0   ? kGreeSwingUp
                          : swingMode == 1 ? kGreeSwingMiddleUp
                          : swingMode == 2 ? kGreeSwingMiddle
                          : swingMode == 3 ? kGreeSwingMiddleDown
                                           : kGreeSwingDown;
        geli.setSwingVertical(automatic, m);
    }
    else
    {
        swingMode = swingMode + 1 > 3 ? 0 : swingMode + 1;
        const uint8_t m = swingMode == 0   ? kGreeSwingAuto
                          : swingMode == 1 ? kGreeSwingDownAuto
                          : swingMode == 2 ? kGreeSwingMiddleAuto
                                           : kGreeSwingUpAuto;
        geli.setSwingVertical(automatic, m);
    }

    Serial.printf(PSTR("set swing , is:\n"));
    Serial.println(automatic);
    Serial.println(geli.getSwingVerticalPosition());

    geli.send();
    handleSendConfig();
}

void handleSendConfig()
{
    Serial.println(geli.toString());
    String json = "";
    json += "{";
    json += "\"status\":\"" + String(geli.getPower()) + "\"";
    json += ",";
    json += "\"temperature\":\"" + String(geli.getTemp()) + "\"";
    json += ",";
    json += "\"mode\":\"" + String(mode) + "\"";
    json += ",";
    json += "\"fan\":\"" + String(fan) + "\"";
    json += ",";
    json += "\"swingAuto\":\"" + String(geli.getSwingVerticalAuto()) + "\"";
    json += ",";
    json += "\"swingMode\":\"" + String(geli.getSwingVerticalPosition()) + "\"";
    json += "}";
    server.send(200, "text/plane", json);

    Serial.printf("json is: ");
    Serial.println(json);
    json = "";

    Serial.println("cmd is end");
}