#include <Arduino.h>

#ifndef _BEMFA_H__
#define _BEMFA_H__

// TCP初始化连接
void do_client_tick();
void start_tcp_client();
void send_to_server(String p);

// 控制函数
void action_handler();

#endif