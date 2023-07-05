#include <Arduino.h>

#include <Ticker.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#ifndef _MWIFI_H__
#define _MWIFI_H__

void loadConfig();
void apConfig();
void restoreFactory();
void saveConfig();
void delayRestart(float t);

void connectWiFi();

#endif