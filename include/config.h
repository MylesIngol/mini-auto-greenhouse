#pragma once

// ─── I2C Bus ───────────────────────────────────────────
#define I2C_SDA 21
#define I2C_SCL 22

// ─── BME280 ────────────────────────────────────────────
#define BME280_I2C_ADDR 0x76  // change to 0x77 if sensor not found

// ─── OLED ──────────────────────────────────────────────
#define OLED_I2C_ADDR   0x3C
#define OLED_WIDTH      128
#define OLED_HEIGHT     64

// ─── Serial ────────────────────────────────────────────
#define SERIAL_BAUD     115200

// ─── Timing ────────────────────────────────────────────
#define SENSOR_READ_INTERVAL_MS 2000