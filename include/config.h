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

// ── Pump ──────────────────────────────────────────────
#define PUMP_PIN                25
#define SOIL_DRY_THRESHOLD      30    // % — start watering below this
#define SOIL_WET_THRESHOLD      60    // % — stop watering above this
#define PUMP_PULSE_ON_MS        2000  // pump on duration per pulse
#define PUMP_PULSE_OFF_MS       10000 // absorption wait between pulses
#define PUMP_MAX_PULSES         5     // safety guard
#define PUMP_COOLDOWN_MS        300000 // 5 min cooldown after session

// ── OLED Paging ───────────────────────────────
#define OLED_PAGE_COUNT 3
#define OLED_ROTATE_INTERVAL_MS 5000

// ── Firmware version ──────────────────────────
#define FW_VERSION "v0.3.0"