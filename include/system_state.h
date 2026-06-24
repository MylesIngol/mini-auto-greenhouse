#pragma once
#include <Arduino.h>

struct SystemState {
    // Environmental
    float tempF       = 0.0f;
    float humidity    = 0.0f;
    float heatIndexF  = 0.0f;

    // Soil & Water
    int   soilPercent = 0;
    bool  waterOk     = false;   // stub until level sensor arrives
    bool  everWatered = false;   // stub until pump arrives
    unsigned long lastWateredMs = 0;

    // System
    bool  wifiConnected = false; // stub until WiFi phase
    char  ipAddress[16] = "--";
    unsigned long uptimeMs = 0;
};