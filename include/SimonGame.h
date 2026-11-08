#pragma once

#include <Arduino.h>
#include "LEDController.h"
#include "Config.h"

class SimonGame
{
public:
    explicit SimonGame(LEDController &leds);

    void start();
    void stop();
    void update();

    void onTouch(int panel);
    void onRelease(int panel);

private:
    enum class State
    {
        Idle,
        ShowingPattern,
        WaitingForInput,
        WaitingForRelease,
        GameOver,
        Won
    };

    LEDController &leds;

    State state = State::Idle;

    int sequence[MAX_SIMON_LENGTH] = {};
    int length = 0;
    int position = 0;

    int patternIndex = 0;
    bool panelLit = false;

    unsigned long lastStepTime = 0;

    static constexpr unsigned long FLASH_TIME = 700;
    static constexpr unsigned long PAUSE_TIME = 400;
    static constexpr unsigned long ROUND_PAUSE = 700;

    bool waitingForRound = false;
    bool touchHeld = false;
    int activePanel = -1;

    void appendToPattern();
    void beginPattern();
    void showSimonHexagon(int panel);
    void gameOver();
};