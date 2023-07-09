#include <Arduino.h>

#ifndef _BEMFA_H__
#define _BEMFA_H__

// TCP初始化连接
void doClientTick();
void startTcpClient();
void sendToServer(String p);

// 控制函数
void actionHandler();

#endif