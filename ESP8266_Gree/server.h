#include <Arduino.h>

#ifndef _SERVER_H__
#define _SERVER_H__

void initWebServer();

void handleIndexPage();
void handleWifiPage();
void handleIRPage();

void handleGetAll();

#endif