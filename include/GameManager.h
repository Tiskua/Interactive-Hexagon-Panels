#pragma once

#include "SimonGame.h"
#include "ReactionGame.h"

enum class GameType
{
    None,
    Simon,
    Reaction
};

class GameManager
{
public:
    GameManager(
        SimonGame &simon,
        ReactionGame &reaction);

    void startGame(GameType type);
    void stopGame();

    void update();

    void onTouch(int panel);
    void onRelease(int panel);

    GameType getCurrentGame() const;

private:
    SimonGame &simon;
    ReactionGame &reaction;

    GameType currentGame = GameType::None;
};