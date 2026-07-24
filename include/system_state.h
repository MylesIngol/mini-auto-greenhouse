#pragma once
#include <Arduino.h>

struct SystemState {
    // Environmental
    float tempF       = 0.0f;
    float humidity    = 0.0f;
    float heatIndexF  = 0.0f;

    // Soil & Water
    int   soilPercent = 0;
    bool  waterOk     = false;
    bool  everWatered = false;
    unsigned long lastWateredMs = 0;

    // Watering event (discrete — set by Watering, consumed by Connectivity, cleared by main.cpp)
    bool wateringEventPending  = false;
    int  wateringEventPulseNum = 0;
    int  wateringEventSoilPct  = 0;

    // System
    bool  wifiConnected = false;
    char  ipAddress[16] = "--";
    unsigned long uptimeMs = 0;
};
