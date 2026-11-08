#pragma once

#include <Arduino.h>
#include <Adafruit_MPR121.h>
#include <functional>

class TouchController
{
public:
    using TouchCallback = std::function<void(int)>;

    bool begin();
    void update();

    void onPress(TouchCallback callback);
    void onRelease(TouchCallback callback);

private:
    Adafruit_MPR121 sensor;

    uint16_t lastTouched = 0;

    TouchCallback pressCallback;
    TouchCallback releaseCallback;
};