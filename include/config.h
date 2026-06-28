#pragma once

// ─── I2C Bus ───────────────────────────────────────────
#define I2C_SDA 21
#define I2C_SCL 22

// ─── BME280 ────────────────────────────────────────────
#define BME280_I2C_ADDR 0x76 // change to 0x77 if sensor not found

// ─── OLED ──────────────────────────────────────────────
#define OLED_I2C_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// ─── Serial ────────────────────────────────────────────
#define SERIAL_BAUD 115200

// ─── Timing ────────────────────────────────────────────
#define SENSOR_READ_INTERVAL_MS 2000

// ─── Soil Moisture Sensor ──────────────────────────────
#define SOIL_SENSOR_PIN 34
#define SOIL_DRY_VALUE 3050 // calibrate this
#define SOIL_WET_VALUE 1450 // calibrate this

// Water Level Sensor
#define WATER_SENSOR_PIN 35
#define WATER_SENSOR_THRESHOLD 500

// ── OLED Paging ───────────────────────────────
#define OLED_PAGE_COUNT 3
#define OLED_ROTATE_INTERVAL_MS 5000

// ── Firmware version ──────────────────────────
#define FW_VERSION "v0.3.0"