# System Architecture

## Overview

```
┌─────────────────────────────────────────────┐
│                  ESP32                       │
│                                             │
│  BME280 ──────► SystemState ◄── SoilSensor  │
│  (I2C)          │                           │
│                 ├──► OLEDDisplay (I2C)      │
│                 └──► Connectivity           │
│                       │                     │
└───────────────────────┼─────────────────────┘
                        │ WiFi / TLS
                        ▼
              HiveMQ Cloud (MQTT broker)
                        │
                        ▼
              InfluxDB Cloud (time-series DB)
                        │
                        ▼
              Grafana Cloud (dashboard)
```

## Firmware Architecture

### Central State Pattern
All modules communicate through a shared `SystemState` struct defined in `include/system_state.h`. Sensors write to it; display and connectivity read from it via `const` reference. No module reaches into another directly.

### Non-blocking Loop
The main loop uses `millis()`-based timers exclusively. No `delay()` calls in steady-state operation. Sensor reads, display updates, and cloud publishes all run on independent intervals.

### Publish Interval
Sensor data is published to MQTT and InfluxDB every 30 seconds. NTP time sync ensures accurate timestamps on every InfluxDB data point.

---

## ESP32

**Microcontroller:** ESP32 DevKit  
**Framework:** Arduino via PlatformIO  
**Clock:** 240 MHz dual-core  
**Flash:** 4MB

### Responsibilities
- Read all sensors on a 2-second interval
- Maintain `SystemState` struct
- Drive OLED display (auto-rotating pages)
- Maintain WiFi connection with auto-reconnect
- Publish sensor data via MQTT (TLS) every 30s
- Write time-series data to InfluxDB every 30s

---

## Sensors

### BME280
- **Interface:** I2C (SDA: GPIO21, SCL: GPIO22, addr: 0x76)
- **Measures:** Temperature (°C → converted to °F), Relative Humidity (%RH)
- **Derived:** Heat index via Rothfusz regression

### Capacitive Soil Moisture Sensor
- **Interface:** ADC (GPIO34)
- **Measures:** Raw ADC value → mapped to 0–100% moisture
- **Calibration:** Dry = 3050, Wet = 1450
- **Note:** Capacitive type chosen over resistive to avoid electrode corrosion

### Water Level Sensor (pending)
- **Type:** DIYables resistive
- **Purpose:** Reservoir empty detection
- **Tradeoff:** Resistive corrosion risk accepted — appropriate for fast-cycle demo species (cat grass). Documented as deliberate scoped decision.

---

## Outputs

### SSD1306 OLED Display (128x64)
- **Interface:** I2C (shared bus with BME280, addr: 0x3C)
- **Pages:** 3-page auto-rotating display (5s interval)
  - Page 1 — Climate: Temp, Humidity, Heat Index
  - Page 2 — Soil & Water: Moisture %, Reservoir status, Last watered
  - Page 3 — System: Uptime, WiFi status, IP address, Firmware version
- **Page indicator:** Filled/outline dot indicators in header

### Peristaltic Pump (pending)
- **Model:** CONQUERALL 5V
- **Control:** PN2222 NPN transistor + 1N4007 flyback diode
- **Why peristaltic:** Self-sealing, self-priming, enables calibrated mL/s dosing. No backflow when unpowered (unlike centrifugal).
- **Power:** Dedicated 5V supply required — ESP32 USB rail insufficient for pump current draw

---

## Cloud Stack

### HiveMQ Cloud
- **Role:** MQTT broker
- **Protocol:** MQTT over TLS (port 8883)
- **Topic:** `greenhouse/sensors`
- **Payload:** JSON — `tempF`, `humidity`, `heatIndexF`, `soilPct`, `waterOk`

### InfluxDB Cloud
- **Role:** Time-series database
- **Measurement:** `environment`
- **Tag:** `device = esp32-greenhouse`
- **Fields:** `temp_f`, `humidity`, `heat_index_f`, `soil_pct`, `water_ok`
- **Timestamp:** NTP-synced epoch seconds

### Grafana Cloud
- **Role:** Dashboard and visualization
- **Datasource:** InfluxDB Cloud (Flux query language)
- **Panels:** Stat panels (current values) + time series graphs per field
- **Refresh:** 30s auto-refresh

---

## GPIO Map

| GPIO | Function |
|------|----------|
| 21 | I2C SDA (BME280 + OLED) |
| 22 | I2C SCL (BME280 + OLED) |
| 34 | Soil moisture sensor (ADC input) |
| TBD | Pump control (transistor base) |
| TBD | Water level sensor |

---

## Key Design Decisions

| Decision | Rationale |
|---|---|
| Peristaltic over centrifugal pump | Self-sealing, self-priming, metered delivery without solenoid valve |
| Capacitive over resistive soil sensor | No electrode corrosion, longer lifespan |
| Resistive water level sensor | Cost/simplicity tradeoff; corrosion acceptable for short demo cycles |
| Local-first WiFi, no remote access | Simplicity; Tailscale available as quick add-on if needed |
| `SystemState` central struct | Decouples modules, prevents circular dependencies, enables clean testing |
| `millis()` non-blocking loop | Allows independent timing of sensors, display, and network without blocking |
| NTP timestamps on InfluxDB points | Required for accurate time-series ordering and Grafana display |