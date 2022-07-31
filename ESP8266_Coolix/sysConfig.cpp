#include <Arduino.h>
#include <EEPROM.h>

#include "sysConfig.h"
#include "global.h"

config_type config;

void saveConfig()
{ //存储配置到"EEPROM"
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
{ //从"EEPROM"加载配置
    // Serial.println("load config");
    // EEPROM.begin(sizeof(config));
    // uint8_t *p = (uint8_t *)(&config);
    // for (uint i = 0; i < sizeof(config); i++)
    // {
    //     *(p + i) = EEPROM.read(i);
    // }

    // // UID = config.uid;
    // // TOPIC = config.topic;
    // Serial.println(config.ssid);
    // Serial.println(config.passwd);
    // Serial.println(config.uid);
    // Serial.println(config.topic);
    // Serial.println("load config end");
}