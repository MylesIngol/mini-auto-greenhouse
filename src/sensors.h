#pragma once

#include "../include/config.h"
#include <Adafruit_BME280.h>

class BME280Sensor
{
public:
    bool begin();
    float readTemperature(); // Celsius
    float readHumidity();    // %RH
    bool isConnected();
    void beginSoil();
    int readSoilRaw();
    int readSoilPercent();

    static float toFahrenheit(float celsius)
    {
        return (celsius * 9.0f / 5.0f) + 32.0f;
    }

private:
    Adafruit_BME280 _bme;
    bool _connected = false;
    int _soilPin = SOIL_SENSOR_PIN;
};