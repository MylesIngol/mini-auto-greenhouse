#include "display.h"
#include "../include/config.h"

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

void OLEDDisplay::showReadings(float tempC, float humidity) {
    _display.clearDisplay();

    // ── Title bar ──────────────────────────────
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("  mini-greenhouse");
    _display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    // ── Temperature ────────────────────────────
    _display.setTextSize(1);
    _display.setCursor(0, 16);
    _display.print("Temp:");

    _display.setTextSize(2);
    _display.setCursor(0, 26);
    _display.print(tempC, 1);
    _display.print(" F");

    // ── Humidity ───────────────────────────────
    _display.setTextSize(1);
    _display.setCursor(0, 48);
    _display.print("Humidity:");
    _display.print(humidity, 1);
    _display.print("%");

    _display.display();
}

void OLEDDisplay::showError(const char* message) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("[ERROR]");
    _display.println(message);
    _display.display();
}