#include "SimonGame.h"

SimonGame::SimonGame(LEDController &leds)
    : leds(leds) {}

void SimonGame::start()
{
    length = 0;
    position = 0;
    patternIndex = 0;

    panelLit = false;
    waitingForRound = false;
    touchHeld = false;
    activePanel = -1;

    leds.clear();

    appendToPattern();
    beginPattern();
}

void SimonGame::stop()
{
    state = State::Idle;
    touchHeld = false;
    activePanel = -1;
    waitingForRound = false;

    leds.clear();
}

void SimonGame::appendToPattern()
{
    if (length >= MAX_SIMON_LENGTH)
        return;

    sequence[length] = random(0, NUM_HEXAGONS);
    length++;
    position = 0;
}

void SimonGame::beginPattern()
{
    state = State::ShowingPattern;

    patternIndex = 0;
    panelLit = false;

    leds.clear();

    lastStepTime = millis();
}

void SimonGame::showSimonHexagon(int panel)
{
    switch (panel)
    {
    case 0:
        leds.setHexagon(panel, CRGB::Red);
        break;

    case 1:
        leds.setHexagon(panel, CRGB::Blue);
        break;

    case 2:
        leds.setHexagon(panel, CRGB::Green);
        break;

    case 3:
        leds.setHexagon(panel, CRGB::Yellow);
        break;

    default:
        leds.setHexagon(panel, CRGB::White);
        break;
    }
}

void SimonGame::update()
{
    unsigned long now = millis();

    if (state == State::WaitingForRelease)
    {
        if (!touchHeld && now - lastStepTime >= ROUND_PAUSE)
        {
            waitingForRound = false;
            appendToPattern();
            beginPattern();
        }

        return;
    }

    if (state != State::ShowingPattern)
        return;

    unsigned long elapsed = now - lastStepTime;

    if (!panelLit)
    {
        if (elapsed >= PAUSE_TIME)
        {
            showSimonHexagon(sequence[patternIndex]);

            panelLit = true;
            lastStepTime = now;
        }
    }
    else
    {
        if (elapsed >= FLASH_TIME)
        {
            leds.clear();

            panelLit = false;
            patternIndex++;

            lastStepTime = now;

            if (patternIndex >= length)
            {
                state = State::WaitingForInput;
                position = 0;
            }
        }
    }
}

void SimonGame::onTouch(int panel)
{
    if (state != State::WaitingForInput)
        return;

    if (panel < 0 || panel >= NUM_HEXAGONS)
        return;

    if (touchHeld)
        return;

    touchHeld = true;
    activePanel = panel;

    if (panel != sequence[position])
    {
        gameOver();
        return;
    }

    showSimonHexagon(panel);
    position++;

    Serial.printf(
        "Simon: Correct input %d/%d\n",
        position,
        length);

    if (position == length)
    {
        Serial.println("Simon: Round Complete!");

        if (length >= MAX_SIMON_LENGTH)
        {
            state = State::Won;
            leds.fill(CRGB::Green);
            Serial.println("Simon: You Win!");
            return;
        }

        waitingForRound = true;
        state = State::WaitingForRelease;
        lastStepTime = millis();
    }
}

void SimonGame::onRelease(int panel)
{
    if (panel != activePanel)
        return;

    touchHeld = false;
    activePanel = -1;

    if (state == State::WaitingForInput ||
        state == State::WaitingForRelease)
    {
        leds.clearHexagon(panel);
    }
}

void SimonGame::gameOver()
{
    state = State::GameOver;
    waitingForRound = false;

    leds.fill(CRGB::Red);

    Serial.println("Simon: Game Over!");
}