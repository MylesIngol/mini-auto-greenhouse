# mini-auto-greenhouse

## Overview

An ESP32-powered automated greenhouse that monitors plant conditions and automatically maintains a healthy growing environment. Target plant: cat grass (wheatgrass/oat/barley), chosen for its fast ~10-14 day growth cycle.

## Goals

- Monitor temperature and humidity
- Monitor soil moisture
- Automatic watering
- Cloud dashboard
- Historical data logging
- Remote monitoring

## Hardware

- ESP32 DevKit
- BME280 (temperature + humidity, I2C)
- SSD1306 OLED (128x64, 3-page auto-rotating display)
- Capacitive soil moisture sensor
- Resistive water level sensor
- Peristaltic pump, switched via NPN transistor (low-side switch) with flyback diode protection
- Water reservoir
- MB102 breadboard power supply (9V center-positive, 1A)

## Project Status

Current Phase:
Phases 1-4 complete. Environmental monitoring, soil monitoring, automatic watering, and cloud dashboard (InfluxDB + Grafana) are implemented and verified, including discrete watering-event logging. Phase 5 (Advanced Controls) not started. Enclosure design in progress.

## Future Features

- Multiple plant zones
- Camera monitoring
- AI plant health analysis
- Mobile app
