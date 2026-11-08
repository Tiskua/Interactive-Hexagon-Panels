#include "ReactionGame.h"
#include "Config.h"

ReactionGame::ReactionGame(LEDController &leds)
    : leds(leds) {}

void ReactionGame::start()
{
    winningPanel = -1;

    countdownDuration = random(2000, 5001);
    countdownStart = millis();

    state = State::Countdown;

    // Red = wait
    leds.fill(CRGB::Red);

    Serial.println("Reaction: Get ready!");
}

void ReactionGame::stop()
{
    state = State::Idle;
    winningPanel = -1;

    leds.clear();
}

void ReactionGame::update()
{
    unsigned long now = millis();

    if (state == State::Countdown)
    {
        if (now - countdownStart >= countdownDuration)
        {
            // Blue = Go
            leds.fill(CRGB::Blue);

            startTime = millis();
            state = State::Waiting;

            Serial.println("Reaction: GO!");
        }
    }
}

void ReactionGame::onTouch(int panel)
{
    if (panel < 0 || panel >= NUM_HEXAGONS)
        return;

    // Player pressed too early
    if (state == State::Countdown)
    {
        state = State::FalseStart;

        leds.fill(CRGB::Red);

        Serial.printf(
            "Reaction: False start by panel %d!\n",
            panel);

        return;
    }

    // Ignore touches unless Go has started
    if (state != State::Waiting)
        return;

    unsigned long reactionTime = millis() - startTime;

    winningPanel = panel;
    state = State::Finished;

    leds.fill(CRGB::Red);
    leds.setHexagon(panel, CRGB::Green);

    Serial.printf(
        "Reaction winner: %d, time: %lu ms\n",
        panel,
        reactionTime);

    if (resultCallback)
    {
        resultCallback(panel, reactionTime);
    }
}

void ReactionGame::onRelease(int panel)
{
    // No action needed
}

void ReactionGame::onResult(ResultCallback callback)
{
    resultCallback = callback;
}