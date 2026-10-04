# ESP32 Automated Mini Greenhouse

An embedded plant-care project that reads environmental sensors, waters dry soil, and logs measurements for remote monitoring. Designed around cat grass using an **ESP32, C++, and PlatformIO**.

## What it does

- Reads temperature and humidity from a BME280.
- Measures soil moisture and checks reservoir water availability.
- Drives a peristaltic pump in timed pulses when soil is dry.
- Displays sensor readings and system status on a three-page OLED display.
- Publishes MQTT telemetry and writes measurements and watering events to InfluxDB for a Grafana dashboard.

## Firmware design

The firmware separates sensor acquisition, watering control, display updates, and network communication into modules that share a system-state structure.

| File | Responsibility |
| --- | --- |
| [src/main.cpp](src/main.cpp) | Initialization and main update loop |
| [src/sensors.cpp](src/sensors.cpp) | Sensor readings and soil-moisture conversion |
| [src/watering.cpp](src/watering.cpp) | Pump pulses, watering decisions, and cooldown |
| [src/display.cpp](src/display.cpp) | OLED pages and status display |
| [src/connectivity.cpp](src/connectivity.cpp) | Wi-Fi, MQTT, time synchronization, and InfluxDB |
| [include/config.h](include/config.h) | Pins, thresholds, calibration, and timing |
| [include/system_state.h](include/system_state.h) | Shared sensor and operating state |

### Watering control

The current configuration starts a pulse below 30% soil moisture when the reservoir check passes. Each pulse lasts 2 seconds, followed by a 10-second absorption wait. The firmware limits a session to five pulses and uses a five-minute cooldown. A separate 60% threshold is used by the session-stop check.

The watering module uses `millis()` timers rather than delays for its pulse timing. These thresholds are calibration settings, not universal plant-care values.

## Hardware

- ESP32 DevKit
- BME280 temperature/humidity sensor
- SSD1306 128 × 64 OLED
- Capacitive soil-moisture sensor
- Resistive reservoir-level sensor
- Peristaltic pump and water reservoir
- NPN transistor pump switch with flyback diode
- MB102 breadboard power supply

Pin assignments and I2C addresses are in [config.h](include/config.h). The pump uses a transistor driver, rather than drawing motor current from a GPIO.

## Build and upload

1. Install PlatformIO and open this project.
2. Copy [include/secrets.example.h](include/secrets.example.h) to `include/secrets.h`.
3. Fill in your Wi-Fi, MQTT, and InfluxDB connection settings.
4. Check the wiring and calibrate the soil sensor values in `include/config.h`.
5. Build, upload, and open the serial monitor:

```bash
pio run
pio run --target upload
pio device monitor
```

The configured board is `esp32dev`, using the Arduino framework and a 115200-baud serial monitor. Dependencies are listed in [platformio.ini](platformio.ini).

## Engineering notes

[Architecture](docs/architecture.md) includes a pump-wiring debugging write-up: the pump initially bypassed the switching transistor, and correcting the current path restored control.

Additional documentation:

- [Hardware inventory](docs/hardware-inventory.md)
- [Requirements](docs/requirements.md)
- [Roadmap](docs/roadmap.md)

## Project status

The existing project notes report environmental sensing, soil monitoring, automatic watering, and InfluxDB/Grafana monitoring as implemented and verified. Enclosure work was in progress; advanced controls, camera monitoring, and multiple plant zones remain future work.

The repository contains firmware and project notes. It does not currently include a reproducible dashboard export or enclosure CAD files.
