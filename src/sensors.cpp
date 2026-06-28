#include "sensors.h"
#include "../include/config.h"
#include <Wire.h>
#include <math.h>

// ── BME280 ────────────────────────────────────────────────────────────────────

bool BME280Sensor::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    _connected = _bme.begin(BME280_I2C_ADDR);
    if (!_connected) Serial.println("[BME280] Not found. Check wiring or I2C address.");
    else             Serial.println("[BME280] Initialized.");
    return _connected;
}

float BME280Sensor::readTemperature() {
    return _connected ? _bme.readTemperature() : -1.0f;
}

float BME280Sensor::readHumidity() {
    return _connected ? _bme.readHumidity() : -1.0f;
}

bool BME280Sensor::isConnected() {
    return _connected;
}

// Rothfusz regression — standard formula used in embedded weather stations
float BME280Sensor::heatIndex(float tempF, float rh) {
    float hi = -42.379f
        + 2.04901523f   * tempF
        + 10.14333127f  * rh
        - 0.22475541f   * tempF * rh
        - 0.00683783f   * tempF * tempF
        - 0.05481717f   * rh * rh
        + 0.00122874f   * tempF * tempF * rh
        + 0.00085282f   * tempF * rh * rh
        - 0.00000199f   * tempF * tempF * rh * rh;
    return hi;
}

// ── SoilSensor ────────────────────────────────────────────────────────────────

void SoilSensor::begin() {
    pinMode(_pin, INPUT);
    Serial.println("[SOIL] Initialized.");
}

int SoilSensor::readRaw() {
    return analogRead(_pin);
}

int SoilSensor::readPercent() {
    int raw = analogRead(_pin);
    int pct = map(raw, SOIL_DRY_VALUE, SOIL_WET_VALUE, 0, 100);
    return constrain(pct, 0, 100);
}

// ── WaterSensor ───────────────────────────────────────────────────────────────

void WaterSensor::begin() {
    pinMode(_pin, INPUT);
    Serial.println("[WATER] Initialized.");
}

int WaterSensor::readRaw() {
    return analogRead(_pin);
}

bool WaterSensor::isOk() {
    return analogRead(_pin) > _threshold;
}