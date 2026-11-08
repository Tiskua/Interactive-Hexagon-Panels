#include "TouchController.h"
#include "Config.h"

bool TouchController::begin()
{
    if (!sensor.begin(0x5A))
        return false;

    sensor.setAutoconfig(true);
    // sensor.setThresholds(6, 6);

    return true;
}

void TouchController::onPress(TouchCallback callback)
{
    pressCallback = callback;
}

void TouchController::onRelease(TouchCallback callback)
{
    releaseCallback = callback;
}

void TouchController::update()
{
    uint16_t current = sensor.touched();

    for (int i = 0; i < NUM_HEXAGONS && i < 12; i++)
    {
        bool touched = current & (1 << i);
        bool previous = lastTouched & (1 << i);

        if (touched && !previous && pressCallback)
        {
            Serial.print("Pressed: ");
            Serial.println(i);
            pressCallback(i);
        }

        if (!touched && previous && releaseCallback)
        {
            Serial.print("Released: ");
            Serial.println(i);
            releaseCallback(i);
        }
    }

    lastTouched = current;
}