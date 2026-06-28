#include "watering.h"
#include "../include/config.h"
#include <Arduino.h>

void Watering::begin()
{
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("[WATER] Watering system initialized.");
}

void Watering::update(SystemState &state)
{
    unsigned long now = millis();

    // ── Cooldown check ────────────────────────────────────────────────────────
    if (_inCooldown)
    {
        if (now - _cooldownStartMs >= PUMP_COOLDOWN_MS)
        {
            _inCooldown = false;
            _pulseCount = 0;
            Serial.println("[PUMP] Cooldown complete.");
        }
        else
        {
            return; // still in cooldown, do nothing
        }
    }

    // ── Pump is currently ON ──────────────────────────────────────────────────
    if (_isPumping)
    {
        // Time to turn off?
        if (now - _pumpStartMs >= PUMP_PULSE_ON_MS)
        {
            _stopPump();
            _lastPulseMs = now;
            _pulseCount++;
            Serial.printf("[PUMP] Pulse %d complete.\n", _pulseCount);

            // Hit max pulses — enter cooldown
            if (_pulseCount >= PUMP_MAX_PULSES)
            {
                Serial.println("[PUMP] Max pulses reached. Starting cooldown.");
                _inCooldown = true;
                _cooldownStartMs = now;
            }
        }
        return;
    }

    // ── Pump is currently OFF ─────────────────────────────────────────────────

    // Already did at least one pulse — wait for absorption before checking again
    if (_pulseCount > 0 && (now - _lastPulseMs < PUMP_PULSE_OFF_MS))
    {
        return;
    }

    // Check if we should stop the session entirely
    if (_pulseCount > 0 && _shouldStopWatering(state))
    {
        Serial.println("[PUMP] Soil wet enough. Starting cooldown.");
        _inCooldown = true;
        _cooldownStartMs = now;
        _pulseCount = 0;
        return;
    }

    // Check if we should start a pulse
    if (_shouldStartWatering(state))
    {
        _startPump(state);
    }
}

// ── Private ───────────────────────────────────────────────────────────────────

bool Watering::_shouldStartWatering(const SystemState &s)
{
    if (!s.waterOk)
    {
        Serial.println("[PUMP] Reservoir empty — skipping.");
        return false;
    }
    return s.soilPercent < SOIL_DRY_THRESHOLD;
}

bool Watering::_shouldStopWatering(const SystemState &s)
{
    return s.soilPercent >= SOIL_WET_THRESHOLD;
}

void Watering::_startPump(SystemState &state)
{
    digitalWrite(PUMP_PIN, HIGH);
    _isPumping = true;
    _pumpStartMs = millis();
    state.everWatered = true;
    state.lastWateredMs = millis();
    Serial.println("[PUMP] Pump ON.");
}

void Watering::_stopPump()
{
    digitalWrite(PUMP_PIN, LOW);
    _isPumping = false;
    Serial.println("[PUMP] Pump OFF.");
}