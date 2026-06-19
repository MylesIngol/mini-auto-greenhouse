# System Architecture

## Main Controller

ESP32

Responsibilities:
- Read sensors
- Make automation decisions
- Drive display
- Connect to WiFi
- Upload data

## Sensors

### BME280
Measures:
- Temperature
- Humidity

### Soil Moisture Sensor
Measures:
- Soil moisture percentage

### Water Level Sensor
Measures:
- Reservoir level

## Outputs

### OLED Display
Shows:
- Temperature
- Humidity
- Soil moisture
- System status

### Water Pump
Used for:
- Automatic watering

## Cloud

Stores:
- Temperature history
- Humidity history
- Watering history
