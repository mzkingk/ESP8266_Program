#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "bemfa.h"
#include "global.h"

// 巴法云服务器地址默认即可
#define TCP_SERVER_ADDR "bemfa.com"
// 服务器端口，tcp创客云端口8344
#define TCP_SERVER_PORT "8344"

// 最大字节数
#define MAX_PACKETSIZE 128
// 设置心跳值
#define KEEPALIVEATIME 30 * 1000

// tcp客户端相关初始化，默认即可
WiFiClient tcp_client;
String client_buff = "";
unsigned int client_buff_idx = 0;
unsigned long client_pre_tick = 0;
unsigned long pre_heart_tick = 0; // 心跳
unsigned long pre_start_tick = 0; // 连接
bool pre_connected = false;

// 发送数据到TCP服务器
void sendToServer(String p)
{
    if (!tcp_client.connected())
    {
        Serial.println("Client is not readly");
        return;
    }
    tcp_client.print(p);
    Serial.print("Send to TCPServer: ");
    Serial.print(p);
}

// 初始化和服务器建立连接
void startTcpClient()
{
    if (tcp_client.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT)))
    {
        Serial.print("\nConnected to server:");
        Serial.printf("%s:%d\r\n", TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT));
        String su(config.cuid);
        String tcpTemp = "";
        tcpTemp = "cmd=1&uid=" + su + "&topic=" + config.ctopic + "\r\n"; // 构建订阅指令
        sendToServer(tcpTemp);                                            // 发送订阅指令
        tcpTemp = "";                                                     // 清空

        pre_connected = true;
        pre_heart_tick = millis();
        tcp_client.setNoDelay(true);
    }
    else
    {
        Serial.print("Failed connected to server:");
        Serial.println(TCP_SERVER_ADDR);
        tcp_client.stop();
        pre_connected = false;
    }
    pre_start_tick = millis();
}

// 检查数据，发送心跳
void doClientTick()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }
    else if (!tcp_client.connected())
    {
        if (pre_connected == true)
        {
            pre_connected = false;
            pre_start_tick = millis();
            Serial.println("TCP Client disconnected.");
            tcp_client.stop();
        }
        else if (millis() - pre_start_tick > 1 * 1000) // 重新连接
        {
            startTcpClient();
        }
        return;
    }

    if (tcp_client.available())
    {
        char c = tcp_client.read(); // 收数据
        client_buff += c;
        client_buff_idx++;
        client_pre_tick = millis();
        if (client_buff_idx >= MAX_PACKETSIZE - 1)
        {
            client_buff_idx = MAX_PACKETSIZE - 2;
            client_pre_tick = client_pre_tick - 200;
        }
        pre_heart_tick = millis();
    }
    if (millis() - pre_heart_tick >= KEEPALIVEATIME)
    {
        pre_heart_tick = millis();
        sendToServer("cmd=0&msg=keep\r\n");
    }
    if ((client_buff.length() < 10) || (millis() - client_pre_tick < 500))
    {
        return;
    }

    tcp_client.flush();
    Serial.print("Buff:");
    Serial.print(client_buff);
    if (client_buff.length() > 16)
    {
        actionHandler();
    }
    client_buff = "";
    client_buff_idx = 0;
}

void actionHandler()
{
    String msg = client_buff.substring(client_buff.indexOf("&msg=") + 5);
    bool status = msg.indexOf("on") >= 0;
    digitalWrite(LEDPIN, status ? HIGH : LOW);      // 端口
    digitalWrite(LED_BUILTIN, status ? LOW : HIGH); // 状态灯，low亮灯，high熄灭
    Serial.printf("its %s\n", status ? "on" : "off");
    msg = "";
}