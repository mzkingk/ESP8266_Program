#include <Arduino.h>

#ifndef _GLOBAL_H__
#define _GLOBAL_H__

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "html.h"
#include "mwifi.h"
#include "server.h"

extern DNSServer dnsServer;
extern ESP8266WebServer server;

#endif