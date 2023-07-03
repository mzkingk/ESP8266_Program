#include <Arduino.h>

#ifndef _BEMFA_H__
#define _BEMFA_H__

// TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

// 控制函数
void actionHandler();

void ledActionHandler(String msg);

#endif