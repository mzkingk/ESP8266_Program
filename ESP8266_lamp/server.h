#include <Arduino.h>

#ifndef _SERVER_H__
#define _SERVER_H__

void initWebServer();

void handleIndexPage();
void handleWifiPage();

void handleCtrlPage();
void handleGetAll();
void ctrlHandler();

#endif