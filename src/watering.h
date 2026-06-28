#pragma once
#include <Arduino.h>
#include "../include/system_state.h"

class Watering {
public:
    void begin();
    void update(SystemState& state);  // call every loop()

private:
    bool _isPumping       = false;
    int  _pulseCount      = 0;
    unsigned long _lastPulseMs    = 0;
    unsigned long _pumpStartMs    = 0;
    unsigned long _cooldownStartMs = 0;
    bool _inCooldown      = false;

    bool _shouldStartWatering(const SystemState& s);
    bool _shouldStopWatering(const SystemState& s);
void _startPump(SystemState& state);
    void _stopPump();
};