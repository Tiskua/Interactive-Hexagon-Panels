#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "Config.h"

class LEDController
{
public:
    void begin();

    void fill(CRGB color);
    void setHexagon(int panel, CRGB color);
    void clearHexagon(int panel);
    void clear();

    void setBrightness(uint8_t brightness);
    void rainbowSections();

private:
    CRGB leds[NUM_LEDS];
    uint8_t hue = 0;
};