#pragma once

#include "../include/config.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OLEDDisplay {
public:
    bool begin();
    void showReadings(float tempC, float humidity);
    void showError(const char* message);

private:
    Adafruit_SSD1306 _display{OLED_WIDTH, OLED_HEIGHT, &Wire, -1};
};