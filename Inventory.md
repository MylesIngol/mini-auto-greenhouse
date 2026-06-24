# Inventory

## Microcontrollers

| Item | Quantity | Status | Notes |
|------|----------|--------|-------|
| ESP32 DevKit | 2 | On Hand | Main controller |
| Arduino Uno | 1 | On Hand | Testing only |

---

## Sensors

| Item | Quantity | Status | Notes |
|------|----------|--------|-------|
| BME280 | 1 | On Hand | Temperature + humidity, I2C (0x76) |
| Capacitive Soil Moisture Sensor | 1 | On Hand | Calibrated: dry=3050, wet=1450 |
| DIYables Water Level Sensor | 1 | Ordered | Resistive type, reservoir monitoring |

---

## Displays

| Item | Quantity | Status | Notes |
|------|----------|--------|-------|
| SSD1306 OLED 128x64 | 3 | On Hand | I2C (0x3C), shared bus with BME280 |

---

## Watering System

| Item | Quantity | Status | Notes |
|------|----------|--------|-------|
| CONQUERALL 5V Peristaltic Pump | 1 | Ordered | Self-priming, self-sealing, metered dosing |
| Silicone Tubing | — | Ordered | Water routing |
| Water Reservoir | — | TBD | Size TBD |

---

## Electronics

| Item | Quantity | Status | Notes |
|------|----------|--------|-------|
| Breadboards | 2 | On Hand | |
| Jumper Wires | Many | On Hand | |
| PN2222 NPN Transistors | Several | On Hand | Pump control switch |
| 1N4007 Diode | Several | On Hand | Flyback protection for pump |
| Resistors | Assorted | On Hand | Base resistor for transistor circuit |

---

## 3D Printed Parts

| Item | Status | Notes |
|------|--------|-------|
| Electronics Enclosure | Not Started | Deferred until all hardware finalized |
| Sensor Mounts | Not Started | |
| Reservoir Brackets | Not Started | |

---

## Future Purchases

| Item | Phase | Notes |
|------|-------|-------|
| Dedicated 5V power supply | Phase 3 | ESP32 USB rail insufficient for pump current |
| Grow light | Phase 6 | |
| Small fan | Phase 6 | Ventilation / humidity control |
| Raspberry Pi Zero 2W | Optional | Local telemetry stack alternative to cloud |
| Ultrasonic fogger | V2 | Active humidity control |