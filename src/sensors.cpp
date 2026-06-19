#include "sensors.h"
#include "../include/config.h"
#include <Wire.h>

bool BME280Sensor::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    _connected = _bme.begin(BME280_I2C_ADDR);

    if (!_connected) {
        Serial.println("[BME280] Not found. Check wiring or I2C address.");
    } else {
        Serial.println("[BME280] Initialized.");
    }

    return _connected;
}

float BME280Sensor::readTemperature() {
    if (!_connected) return -1.0f;
    return _bme.readTemperature();
}

float BME280Sensor::readHumidity() {
    if (!_connected) return -1.0f;
    return _bme.readHumidity();
}

bool BME280Sensor::isConnected() {
    return _connected;
}