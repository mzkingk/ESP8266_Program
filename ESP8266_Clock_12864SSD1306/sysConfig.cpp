#include <Arduino.h>

#include <EEPROM.h>

#include "sysConfig.h"
#include "global.h"

config_type config;

void saveConfig()
{ //存储配置到"EEPROM"
    Serial.println("save config");
    EEPROM.begin(sizeof(config));
    uint8_t *p = (uint8_t *)(&config);
    for (uint i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit(); //此操作会消耗flash写入次数
}

void loadConfig()
{ //从"EEPROM"加载配置
    Serial.println("load config");
    EEPROM.begin(sizeof(config));
    uint8_t *p = (uint8_t *)(&config);
    for (uint i = 0; i < sizeof(config); i++)
    {
        *(p + i) = EEPROM.read(i);
    }

    UID = config.uid.length() > 0 ? config.uid : UID;
    TOPIC = config.topic.length() > 0 ? config.topic : TOPIC;
    Serial.printf("print config, uid is:%s\n", config.uid);
    Serial.printf("print config, topic is:%s\n", config.topic);
}