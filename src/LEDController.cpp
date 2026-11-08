#include "LEDController.h"

void LEDController::begin()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(
        leds, NUM_LEDS);

    FastLED.setBrightness(100);
    clear();
}

void LEDController::fill(CRGB color)
{
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
}

void LEDController::setHexagon(int panel, CRGB color)
{
    if (panel < 0 || panel >= NUM_HEXAGONS)
        return;

    int start = panel * LEDS_PER_HEXAGON;

    for (int i = start; i < start + LEDS_PER_HEXAGON; i++)
    {
        leds[i] = color;
    }

    FastLED.show();
}

void LEDController::clearHexagon(int panel)
{
    setHexagon(panel, CRGB::Black);
}

void LEDController::clear()
{
    FastLED.clear();
    FastLED.show();
}

void LEDController::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void LEDController::rainbowSections()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        int position = i % LEDS_PER_HEXAGON;

        leds[i] = CHSV(
            hue + (position * 256 / LEDS_PER_HEXAGON),
            255,
            255);
    }

    FastLED.show();
    hue++;
}