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
- Switched via NPN transistor (low-side switch), with a 10kΩ base pull-down and a 1N4007 flyback diode across the pump terminals for back-EMF protection

## Cloud

Stores:
- Temperature history
- Humidity history
- Watering history

---

## Debugging Postmortem: Continuous Pump Operation (Phase 3)

**Symptom:** Pump ran continuously regardless of GPIO/transistor switching state — firmware logic appeared correct, but the pump never turned off.

**Root cause:** Wiring topology error. The pump's negative lead was wired directly to ground, bypassing the transistor entirely. The transistor was present in the circuit but never in the current path, so it had no ability to switch the pump on or off — it was just sitting there while the pump ran straight off the supply.

**Fix:** Rewired as a proper low-side NPN switch:
- Pump (−) → transistor collector
- Transistor emitter → ground
- 10kΩ pull-down resistor on the transistor base (prevents floating-base false triggering)
- 1N4007 flyback diode across the pump terminals (protects the transistor from back-EMF generated when the pump coil switches off)

**Lesson:** When output hardware doesn't respond to switching, verify the wiring topology before assuming a firmware bug. A transistor that's present in the circuit but wired around the load contributes nothing — trace the actual current path first, then check code.
