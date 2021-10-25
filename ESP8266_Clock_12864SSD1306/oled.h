#include <Arduino.h>
#ifndef _OLED_H__
#define _OLED_H__

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
void oledClockDisplay();
void sendCommand(int command, int value);
void initdisplay();

extern boolean isNTPConnected;

extern unsigned char xing[] U8X8_PROGMEM;
extern unsigned char liu[] U8X8_PROGMEM;

#endif