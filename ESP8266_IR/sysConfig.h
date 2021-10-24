#include <Arduino.h>

#ifndef _SYS_CONFIG_H__
#define _SYS_CONFIG_H__

typedef struct
{                 // 存储配置结构体
    String uid;   // 巴法云uid
    String topic; // 巴法云主题
} config_type;
extern config_type config;

void saveConfig();
void loadConfig();

#endif