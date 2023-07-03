#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "bemfa.h"
#include "global.h"

// 巴法云服务器地址默认即可
#define TCP_SERVER_ADDR "bemfa.com"
// 服务器端口，tcp创客云端口8344
#define TCP_SERVER_PORT "8344"

// 最大字节数
#define MAX_PACKETSIZE 64
// 设置心跳值30s
#define KEEPALIVEATIME 15 * 1000

// tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;    // 心跳
unsigned long preTCPStartTick = 0; // 连接
bool preTCPConnected = false;

/*
 *发送数据到TCP服务器
 */
void sendtoTCPServer(String p)
{

    if (!TCPclient.connected())
    {
        Serial.println("Client is not readly");
        return;
    }
    TCPclient.print(p);
    Serial.println("[Send to TCPServer]:String");
    Serial.print(p);
}

/*
 *初始化和服务器建立连接
 */
void startTCPClient()
{
    if (TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT)))
    {
        Serial.print("\nConnected to server:");
        Serial.printf("%s:%d\r\n", TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT));

        String tcpTemp = "";                                       // 初始化字符串
        tcpTemp = "cmd=1&uid=" + UID + "&topic=" + TOPIC + "\r\n"; // 构建订阅指令
        sendtoTCPServer(tcpTemp);                                  // 发送订阅指令
        tcpTemp = "";                                              // 清空

        preTCPConnected = true;
        preHeartTick = millis();
        TCPclient.setNoDelay(true);
    }
    else
    {
        Serial.print("Failed connected to server:");
        Serial.println(TCP_SERVER_ADDR);
        TCPclient.stop();
        preTCPConnected = false;
    }
    preTCPStartTick = millis();
}

/*
 *检查数据，发送心跳
 */
void doTCPClientTick()
{
    // 检查是否断开，断开后重连
    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!TCPclient.connected())
    { // 断开重连

        if (preTCPConnected == true)
        {

            preTCPConnected = false;
            preTCPStartTick = millis();
            Serial.println();
            Serial.println("TCP Client disconnected.");
            TCPclient.stop();
        }
        else if (millis() - preTCPStartTick > 1 * 1000) // 重新连接
            startTCPClient();
    }
    else
    {
        if (TCPclient.available())
        { // 收数据
            char c = TCPclient.read();
            TcpClient_Buff += c;
            TcpClient_BuffIndex++;
            TcpClient_preTick = millis();

            if (TcpClient_BuffIndex >= MAX_PACKETSIZE - 1)
            {
                TcpClient_BuffIndex = MAX_PACKETSIZE - 2;
                TcpClient_preTick = TcpClient_preTick - 200;
            }
            preHeartTick = millis();
        }
        if (millis() - preHeartTick >= KEEPALIVEATIME)
        { // 保持心跳
            preHeartTick = millis();
            Serial.println("--Keep alive:");
            sendtoTCPServer("cmd=0&msg=keep\r\n");
        }
    }
    if ((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick >= 1000))
    { // data ready
        TCPclient.flush();
        Serial.print("Buff:");
        Serial.print(TcpClient_Buff);

        actionHandler();

        TcpClient_Buff = "";
        TcpClient_BuffIndex = 0;
    }
}

void actionHandler()
{
    String msg = TcpClient_Buff.substring(TcpClient_Buff.indexOf("&msg=") + 5);

    Serial.printf(PSTR("msg: %s"), msg);
    if (msg.indexOf("on") >= 0)
    {
        digitalWrite(LEDPIN, LOW);
        Serial.printf("open success");
    }
    else if (msg.indexOf("off") >= 0)
    {
        digitalWrite(LEDPIN, HIGH);
        Serial.printf("close success");
    }
    msg = "";
}