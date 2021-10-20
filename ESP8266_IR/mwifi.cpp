#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "mwifi.h"

String wiFiScan() {
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t* bssid;
    int32_t channel;
    bool hidden;
    int scanResult;

    Serial.println(F("Starting WiFi scan..."));

    scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (scanResult == 0) {
        Serial.println(F("No networks found"));
    } else if (scanResult > 0) {
        Serial.printf(PSTR("%d networks found:\n"), scanResult);

        String list = String("[\"");
        for (int8_t i = 0; i < scanResult; i++) {
            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

            Serial.printf(PSTR("               %ddBm %s\n"), rssi, ssid.c_str());
            yield();

            if (ssid.length() > 0) {
                String str = String((int) rssi);
                list += str;
                list += "dBm ";
                list += ssid.c_str();
                list += "\",\"";
            }
        }

        if (list.length() > 2) {
            list.remove(list.length() - 2);
        }
        list += "]";

        return list;
    } else {
        Serial.printf(PSTR("WiFi scan error %d"), scanResult);
    }
    return "[]";
}