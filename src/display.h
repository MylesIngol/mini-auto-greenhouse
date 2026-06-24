#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "../include/config.h"
#include "../include/system_state.h"

class OLEDDisplay {
public:
    bool begin();
    void update(const SystemState& state);  // call every loop()
    void showError(const char* message);

private:
    Adafruit_SSD1306 _display{OLED_WIDTH, OLED_HEIGHT, &Wire, -1};

    uint8_t          _currentPage = 0;
    unsigned long    _lastRotateMs = 0;

    void _showClimate(const SystemState& s);
    void _showSoilWater(const SystemState& s);
    void _showSystem(const SystemState& s);
    void _drawHeader(const char* title, uint8_t page);
    void _drawFooter(const SystemState& s);
};