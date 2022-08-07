#include <Arduino.h>

#include <EEPROM.h>

#include "sysConfig.h"
#include "global.h"

config_type config;

void saveConfig()
{
    // Serial.println("save config");
    // EEPROM.begin(sizeof(config));
    // uint8_t *p = (uint8_t *)(&config);
    // for (uint i = 0; i < sizeof(config); i++)
    // {
    //     EEPROM.write(i, *(p + i));
    // }
    // EEPROM.commit(); //此操作会消耗flash写入次数
}

void loadConfig()
{
    // Serial.println("load config");
    // EEPROM.begin(sizeof(config));
    // uint8_t *p = (uint8_t *)(&config);
    // for (uint i = 0; i < sizeof(config); i++)
    // {
    //     *(p + i) = EEPROM.read(i);
    // }
}