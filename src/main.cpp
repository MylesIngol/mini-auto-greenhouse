#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"
#include "display.h"
#include "connectivity.h"
#include "../include/config.h"
#include "../include/system_state.h"
#include "watering.h"
Watering watering;
WaterSensor   waterSensor;
BME280Sensor  envSensor;
SoilSensor    soilSensor;
OLEDDisplay   display;
Connectivity  connectivity;
SystemState   state;

unsigned long lastSensorReadMs = 0;

void setup()
{
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    Serial.println("=== mini-auto-greenhouse " FW_VERSION " ===");
watering.begin();
    waterSensor.begin();
    envSensor.begin();
    soilSensor.begin();
    display.begin();

    if (!envSensor.isConnected())
    {
        display.showError("BME280 not found");
    }

    if (connectivity.begin())
    {
        state.wifiConnected = true;
        WiFi.localIP().toString().toCharArray(state.ipAddress, 16);
    }
}

void loop()
{
    unsigned long now = millis();
    state.uptimeMs = now;

    if (now - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS)
    {
        lastSensorReadMs = now;

        if (envSensor.isConnected())
        {
            float tempC     = envSensor.readTemperature();
            state.tempF     = BME280Sensor::toFahrenheit(tempC);
            state.humidity  = envSensor.readHumidity();
            state.heatIndexF = BME280Sensor::heatIndex(state.tempF, state.humidity);

            Serial.printf("[ENV] Temp: %.1f F  Humidity: %.1f%%  HeatIdx: %.1f F\n",
                          state.tempF, state.humidity, state.heatIndexF);
        }

        state.soilPercent = soilSensor.readPercent();
        Serial.printf("[SOIL] Raw: %d  Moisture: %d%%\n",
                      soilSensor.readRaw(), state.soilPercent);

        state.waterOk = waterSensor.isOk();
        Serial.printf("[WATER] Raw: %d  OK: %s\n",
                      waterSensor.readRaw(),
                      state.waterOk ? "yes" : "no");
    }
watering.update(state);
    connectivity.update(state);
    display.update(state);
}