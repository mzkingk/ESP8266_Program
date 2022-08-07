#include <Arduino.h>
#ifndef _OLED_H__
#define _OLED_H__

time_t getNtpTime();
void oledClockDisplay();
void initdisplay();

extern unsigned char xing[] U8X8_PROGMEM;
extern unsigned char liu[] U8X8_PROGMEM;

#endif