#include <Arduino.h>

#ifndef _IRServer_H__
#define _IRServer_H__

void handleTemperature();
void handleIROnOff();
void handleModeChange();

void handleSend();

#endif