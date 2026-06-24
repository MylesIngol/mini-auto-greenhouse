# mini-auto-greenhouse

An ESP32-powered automated greenhouse that monitors plant conditions in real time and automatically maintains a healthy growing environment. Built as a portfolio project demonstrating embedded systems, IoT cloud integration, and full-stack data pipelines.

## Live Stack

```
ESP32 → HiveMQ Cloud (MQTT) → InfluxDB Cloud → Grafana Dashboard
```

Sensor data is published every 30 seconds, stored as a time series, and visualized on a live cloud dashboard.

## Features

### Complete
- Temperature, humidity, and heat index monitoring (BME280)
- Capacitive soil moisture monitoring (calibrated)
- 3-page auto-rotating OLED display (Climate / Soil & Water / System)
- WiFi connectivity with non-blocking firmware loop
- MQTT publishing to HiveMQ Cloud (TLS)
- Time-series data ingestion to InfluxDB Cloud (NTP-timestamped)
- Live Grafana dashboard with stat panels and historical graphs
- Modular firmware architecture with shared `SystemState` struct

### In Progress
- Automated watering system (peristaltic pump + water level sensor — hardware arriving)

### Planned
- Watering event annotations in Grafana
- Embedded Grafana dashboard in custom web interface
- Remote pump control via MQTT
- FreeRTOS refactor
- Custom PCB design

## Hardware

| Component | Purpose |
|---|---|
| ESP32 DevKit | Main controller |
| BME280 | Temperature + humidity (I2C) |
| SSD1306 OLED 128x64 | Local display (I2C) |
| Capacitive soil moisture sensor | Soil monitoring (GPIO34) |
| CONQUERALL 5V peristaltic pump | Automated watering (arriving) |
| DIYables water level sensor | Reservoir monitoring (arriving) |
| PN2222 transistor + 1N4007 diode | Pump control circuit |

## Wiring

| Signal | GPIO |
|---|---|
| I2C SDA (BME280 + OLED) | 21 |
| I2C SCL (BME280 + OLED) | 22 |
| Soil moisture sensor | 34 |

## Project Structure

```
mini-auto-greenhouse/
├── include/
│   ├── config.h              # Pin definitions, timing constants
│   ├── system_state.h        # Shared state struct across all modules
│   ├── secrets.h             # WiFi + cloud credentials (gitignored)
│   └── secrets.example.h    # Credential template
├── src/
│   ├── main.cpp              # Setup, loop, sensor read timing
│   ├── sensors.cpp/.h        # BME280Sensor + SoilSensor classes
│   ├── display.cpp/.h        # OLEDDisplay — 3-page auto-rotate
│   └── connectivity.cpp/.h  # WiFi, MQTT, InfluxDB
└── platformio.ini
```

## Setup

### 1. Clone and install dependencies
```bash
git clone https://github.com/MylesIngol/mini-auto-greenhouse.git
cd mini-auto-greenhouse
```
PlatformIO will automatically install all library dependencies on first build.

### 2. Configure credentials
```bash
cp include/secrets.example.h include/secrets.h
```
Fill in your values in `secrets.h`:
- WiFi SSID + password
- HiveMQ Cloud hostname, username, password
- InfluxDB Cloud URL, token, org, bucket

### 3. Flash
```bash
pio run --target upload
```
Monitor output:
```bash
pio device monitor
```

## Firmware Architecture

All modules communicate through a central `SystemState` struct. Sensors write to it, display and connectivity read from it. No module reaches into another directly.

```
main.cpp
  ├── BME280Sensor  →  state.tempF / humidity / heatIndexF
  ├── SoilSensor    →  state.soilPercent
  ├── OLEDDisplay   ←  reads SystemState (const ref)
  └── Connectivity  ←  reads SystemState, publishes every 30s
```

The loop uses `millis()`-based non-blocking timers throughout — no `delay()` calls outside of setup.

## Target Species

Currently growing cat grass (wheatgrass/oat grass) — chosen for its fast 10–14 day growth cycle and well-documented environmental requirements:

| Parameter | Target Range |
|---|---|
| Temperature | 65–75°F (18–24°C) |
| Humidity | 50–70% RH |
| Soil moisture | 40–70% |

## Roadmap

See [Roadmap.md](Roadmap.md) for full phase breakdown.