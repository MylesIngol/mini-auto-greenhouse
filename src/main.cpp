#include <Arduino.h>
#include "sensors.h"
#include "display.h"
#include "../include/config.h"

BME280Sensor sensor;
OLEDDisplay display;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  Serial.println("=== mini-auto-greenhouse ===");

  sensor.begin();
  display.begin();
  sensor.beginSoil();

  if (!sensor.isConnected())
  {
    display.showError("BME280 not found");
  }
}

void loop()
{
  if (sensor.isConnected())
  {
    float temp = BME280Sensor::toFahrenheit(sensor.readTemperature());
    float humidity = sensor.readHumidity();

    Serial.printf("[ENV] Temp: %.1f F  Humidity: %.1f %%\n", temp, humidity);
    display.showReadings(temp, humidity);
  }
  int soilRaw = sensor.readSoilRaw();
  int soilPercent = sensor.readSoilPercent();

  Serial.printf("[SOIL] Raw: %d  Moisture: %d%%\n", soilRaw, soilPercent);

  delay(SENSOR_READ_INTERVAL_MS);
}