#ifndef WIFI_UTILITY_H
#define WIFI_UTILITY_H

#include "HardwareSerial.h"
#include <ArduinoJson.h>
#include <WiFi.h>

//----- Wi-Fi configuration -----
    extern const char *ssid;
    extern const char *password;
    extern const uint16_t port;
    extern const char *server_ip;

    bool connectToWiFi(uint32_t timeout_ms);
    void debugWiFiConnection(uint8_t state);

    void setup_WiFi_server();
    bool setup_WiFi_client();

    void sendJson(float weights[], uint16_t weight_number, WiFiClient &conn);

    uint8_t syncWeights_server(uint16_t weight_number, float FlatWeights[]);
    uint8_t syncWeights_client(uint16_t weight_number, float FlatWeights[]);

#endif WIFI_UTILITY_H