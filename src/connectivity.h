#pragma once
#include <Arduino.h>
#include "../include/system_state.h"

class Connectivity {
public:
    bool begin();                          // connect WiFi, MQTT, init InfluxDB
    void update(const SystemState& state); // call every loop — handles publish + reconnect
    bool isWifiConnected();

private:
    bool _mqttConnected = false;

    void _connectWifi();
    void _connectMQTT();
    void _publishMQTT(const SystemState& s);
    void _publishInflux(const SystemState& s);
};