#include <Arduino.h>
#ifndef _SERVER_H__
#define _SERVER_H__

void handleGetAll();
void handleWifiPage();
void initWebServer();
void handleWifiPost();
void saveConfig();

#endif