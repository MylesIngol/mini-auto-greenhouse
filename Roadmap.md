# Greenhouse Roadmap

## Phase 1 - Environmental Monitoring ✅
- [x] ESP32 setup
- [x] BME280 integration (temperature + humidity)
- [x] SSD1306 OLED display
- [x] Sensor readings displayed on OLED

## Phase 2 - Soil Monitoring ✅
- [x] Capacitive soil moisture sensor (GPIO34)
- [x] Dry/wet calibration (dry: 3050, wet: 1450)
- [x] Soil moisture percentage displayed on OLED

## Phase 3 - Watering System 🔄
- [x] Water reservoir planned
- [x] Pump selected (CONQUERALL 5V peristaltic) — ordered
- [x] Water level sensor selected (DIYables resistive) — ordered
- [ ] Pump control circuit (PN2222 + 1N4007 flyback diode)
- [ ] Water level sensor integration
- [ ] Automatic watering state machine with hysteresis
- [ ] Watering events logged to InfluxDB

## Phase 4 - Cloud Dashboard ✅
- [x] WiFi connection (non-blocking)
- [x] MQTT publishing to HiveMQ Cloud (TLS, every 30s)
- [x] NTP time sync for accurate InfluxDB timestamps
- [x] InfluxDB Cloud time-series ingestion
- [x] Grafana Cloud dashboard (stat panels + historical graphs)

## Phase 5 - Web Interface 🔄
- [ ] Embedded Grafana panels in custom webpage
- [ ] Remote pump trigger via MQTT
- [ ] Moisture threshold configuration

## Phase 6 - Advanced Controls
- [ ] Fan control
- [ ] Grow light control
- [ ] Watering event annotations in Grafana
- [ ] Push notifications (moisture low, reservoir empty)

## Phase 7 - Smart Features
- [ ] Timelapse camera (spare iPhone + ffmpeg)
- [ ] FreeRTOS refactor
- [ ] Custom PCB design
- [ ] Plant health detection
- [ ] AI recommendations

## V2 - Advanced Greenhouse
- [ ] Active humidity control (ultrasonic fogger + ventilation fan)
- [ ] PID/hysteresis control loop
- [ ] More demanding plant species
- [ ] Hydroponics exploration