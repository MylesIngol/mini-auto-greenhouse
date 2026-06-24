#include "display.h"

bool OLEDDisplay::begin() {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("[OLED] Not found. Check wiring or I2C address.");
        return false;
    }
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);
    Serial.println("[OLED] Initialized.");
    return true;
}

void OLEDDisplay::update(const SystemState& state) {
    unsigned long now = millis();

    // Auto-rotate page
    if (now - _lastRotateMs >= OLED_ROTATE_INTERVAL_MS) {
        _currentPage = (_currentPage + 1) % OLED_PAGE_COUNT;
        _lastRotateMs = now;
    }

    _display.clearDisplay();

    switch (_currentPage) {
        case 0: _showClimate(state);   break;
        case 1: _showSoilWater(state); break;
        case 2: _showSystem(state);    break;
    }

    _display.display();
}

// ── Shared header ─────────────────────────────────────────────────────────────
void OLEDDisplay::_drawHeader(const char* title, uint8_t page) {
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.print(title);

    // Page indicator dots — right aligned
    for (uint8_t i = 0; i < OLED_PAGE_COUNT; i++) {
        int x = 128 - (OLED_PAGE_COUNT - i) * 8;
        if (i == page) {
            _display.fillCircle(x, 3, 2, SSD1306_WHITE);  // filled = current
        } else {
            _display.drawCircle(x, 3, 2, SSD1306_WHITE);  // outline = other
        }
    }

    _display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
}

// ── Page 1: Climate ───────────────────────────────────────────────────────────
void OLEDDisplay::_showClimate(const SystemState& s) {
    _drawHeader("  Climate", 0);

    _display.setTextSize(1);
    _display.setCursor(0, 14);
    _display.print("Temp:");
    _display.setTextSize(2);
    _display.setCursor(36, 12);
    _display.print(s.tempF, 1);
    _display.print(" F");

    _display.setTextSize(1);
    _display.setCursor(0, 34);
    _display.print("Humidity:  ");
    _display.print(s.humidity, 1);
    _display.print("%");

    _display.setCursor(0, 46);
    _display.print("Heat Idx:  ");
    _display.print(s.heatIndexF, 1);
    _display.print(" F");

    _display.setCursor(0, 56);
}

// ── Page 2: Soil & Water ──────────────────────────────────────────────────────
void OLEDDisplay::_showSoilWater(const SystemState& s) {
    _drawHeader("  Soil & Water", 1);

    _display.setTextSize(1);
    _display.setCursor(0, 14);
    _display.print("Moisture:  ");
    _display.print(s.soilPercent);
    _display.print("%");

    _display.setCursor(0, 26);
    _display.print("Reservoir: ");
    _display.print(s.waterOk ? "OK" : "--");

    _display.setCursor(0, 38);
    _display.print("Watered:   ");
    if (s.everWatered) {
        unsigned long secAgo = (millis() - s.lastWateredMs) / 1000;
        if (secAgo < 60) {
            _display.print(secAgo);
            _display.print("s ago");
        } else {
            _display.print(secAgo / 60);
            _display.print("m ago");
        }
    } else {
        _display.print("--");
    }
}

// ── Page 3: System ────────────────────────────────────────────────────────────
void OLEDDisplay::_showSystem(const SystemState& s) {
    _drawHeader("  System", 2);

    // Format uptime as HH:MM:SS
    unsigned long totalSec = s.uptimeMs / 1000;
    unsigned int  h = totalSec / 3600;
    unsigned int  m = (totalSec % 3600) / 60;
    unsigned int  sec = totalSec % 60;

    char uptimeBuf[12];
    snprintf(uptimeBuf, sizeof(uptimeBuf), "%02u:%02u:%02u", h, m, sec);

    _display.setTextSize(1);
    _display.setCursor(0, 14);
    _display.print("Uptime: ");
    _display.print(uptimeBuf);

    _display.setCursor(0, 26);
    _display.print("WiFi:   ");
    _display.print(s.wifiConnected ? "Connected" : "--");

    _display.setCursor(0, 38);
    _display.print("IP:     ");
    _display.print(s.ipAddress);

    _display.setCursor(0, 52);
    _display.print(FW_VERSION);
}

// ── Error screen ──────────────────────────────────────────────────────────────
void OLEDDisplay::showError(const char* message) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("[ERROR]");
    _display.println(message);
    _display.display();
}