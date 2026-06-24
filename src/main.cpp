#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"
#include "display.h"
#include "connectivity.h"
#include "../include/config.h"
#include "../include/system_state.h"

BME280Sensor  envSensor;
SoilSensor    soilSensor;
OLEDDisplay   display;
Connectivity  connectivity;
SystemState   state;

unsigned long lastSensorReadMs = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    Serial.println("=== mini-auto-greenhouse " FW_VERSION " ===");

    envSensor.begin();
    soilSensor.begin();
    display.begin();

    if (!envSensor.isConnected()) {
        display.showError("BME280 not found");
    }

    // WiFi + cloud — non-fatal if it fails
    if (connectivity.begin()) {
        state.wifiConnected = true;
        WiFi.localIP().toString().toCharArray(state.ipAddress, 16);
    }
}

void loop() {
    unsigned long now = millis();
    state.uptimeMs = now;

    // ── Sensor reads ──────────────────────────────────────────────────────────
    if (now - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS) {
        lastSensorReadMs = now;

        if (envSensor.isConnected()) {
            float tempC      = envSensor.readTemperature();
            state.tempF      = BME280Sensor::toFahrenheit(tempC);
            state.humidity   = envSensor.readHumidity();
            state.heatIndexF = BME280Sensor::heatIndex(state.tempF, state.humidity);

            Serial.printf("[ENV] Temp: %.1f F  Humidity: %.1f%%  HeatIdx: %.1f F\n",
                          state.tempF, state.humidity, state.heatIndexF);
        }

        state.soilPercent = soilSensor.readPercent();
        Serial.printf("[SOIL] Raw: %d  Moisture: %d%%\n",
                      soilSensor.readRaw(), state.soilPercent);
    }

    // ── Connectivity ──────────────────────────────────────────────────────────
    connectivity.update(state);

    // ── Display ───────────────────────────────────────────────────────────────
    display.update(state);
}