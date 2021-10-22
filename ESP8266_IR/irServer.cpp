#include <Arduino.h>

#include <IRremoteESP8266.h> //ESP8266红外控制库
#include <IRsend.h>          //红外发送库
#include <ir_Gree.h>         //添加格力空调的库

#include "irServer.h"
#include "global.h"

int onOffFlag = 0;
int temperature = 26;
int mode = 0;

void handleIROnOff()
{
    Serial.println("successfully enter handle! on off");
    onOffFlag++;
    Serial.printf(PSTR("flag is :%s\n"), onOffFlag % 2 != 0 ? "on" : "off");
    onOffFlag % 2 != 0 ? ac.on() : ac.off();

    ac.send();
    handleSendConfig();
}

void handleTemperature()
{
    Serial.println("enter handle! temperature");
    String arg = server.arg(0);

    arg == String('1') ? temperature++ : temperature--;
    temperature = temperature < 16 ? 16 : temperature;
    temperature = temperature > 30 ? 30 : temperature;

    Serial.printf(PSTR("temperature is :%d, value is:%s\n"), temperature, arg);

    ac.setTemp(temperature);
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

void handleSendConfig()
{
    Serial.println(ac.toString());
    String json = "";
    json += "{";
    json += "\"status\":\"" + String(onOffFlag % 2 == 0) + "\"";
    json += ",";
    json += "\"temperature\":\"" + String(temperature) + "\"";
    json += ",";
    json += "\"mode\":\"" + String(mode) + "\"";
    json += "}";
    server.send(200, "text/plane", json);

    Serial.printf("json is: ");
    Serial.println(json);
    json = "";

    Serial.println("cmd is end");
}