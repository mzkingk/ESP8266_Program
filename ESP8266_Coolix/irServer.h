#include <Arduino.h>

#ifndef _IRServer_H__
#define _IRServer_H__

void handleTemperature();
void handleIROnOff();
void handleModeChange();
void handleFanChange();

void swingAuto();

void handleSendConfig();
String getJson();

void irActionHandler(String msg);

#endif