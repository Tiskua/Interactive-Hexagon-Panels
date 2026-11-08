#pragma once

#include <Arduino.h>
#include <functional>

#include "LEDController.h"

class ReactionGame
{
public:
    using ResultCallback =
        std::function<void(int, unsigned long)>;

    explicit ReactionGame(LEDController &leds);

    void start();
    void stop();
    void update();

    void onTouch(int panel);
    void onRelease(int panel);

    void onResult(ResultCallback callback);

private:
    enum class State
    {
        Idle,
        Countdown,
        Waiting,
        Finished,
        FalseStart
    };

    LEDController &leds;

    State state = State::Idle;

    int winningPanel = -1;

    unsigned long countdownStart = 0;
    unsigned long countdownDuration = 0;
    unsigned long startTime = 0;

    ResultCallback resultCallback;
};