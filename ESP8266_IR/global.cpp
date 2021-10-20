#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "global.h"

DNSServer dnsServer;
ESP8266WebServer server(80);