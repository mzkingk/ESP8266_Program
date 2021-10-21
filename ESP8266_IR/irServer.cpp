#include <Arduino.h>

#include <IRremoteESP8266.h> //ESP8266红外控制库
#include <IRsend.h>          //红外发送库
#include <ir_Gree.h>         //添加格力空调的库

#include "irServer.h"
#include "global.h"

int onOffFlag = 0;
void handleIROnOff()
{
    Serial.println("successfully enter handle! on off");
    onOffFlag++;
    Serial.printf("cmd is :");
    Serial.println(onOffFlag);
    if (onOffFlag % 2 != 0)
    {
        ac.on();
        Serial.println("cmd is on");
    }
    else
    {
        ac.off();
        Serial.println("cmd is off");
    }

    Serial.println(ac.toString()); //显示发送的空调开机红外编码
    ac.send();                     //发送红外命令
    server.send(200, "text/plane", "1");
    Serial.println("cmd is end");
}