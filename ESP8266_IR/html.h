#include <Arduino.h>
#ifndef _HTML_H__
#define _HTML_H__

void initWifiHtml(char *str);

// 配网页面
#define MULTI_LINE_STRING(a) #a
extern char *wifi_html;

#endif