#pragma once
#include "../include/config.h"
#include <Adafruit_BME280.h>

class BME280Sensor {
public:
    bool begin();
    float readTemperature();  // Celsius
    float readHumidity();     // %RH
    bool  isConnected();

    static float toFahrenheit(float c) { return (c * 9.0f / 5.0f) + 32.0f; }
    static float heatIndex(float tempF, float humidity);

private:
    Adafruit_BME280 _bme;
    bool _connected = false;
};

class SoilSensor {
public:
    void begin();
    int readRaw();
    int readPercent();

private:
    int _pin = SOIL_SENSOR_PIN;
};

class WaterSensor {
public:
    void begin();
    int  readRaw();
    bool isOk();  // true = water detected

private:
    int _pin       = WATER_SENSOR_PIN;
    int _threshold = WATER_SENSOR_THRESHOLD;
};