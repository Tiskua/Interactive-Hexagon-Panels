#include "GameManager.h"

GameManager::GameManager(
    SimonGame &simon,
    ReactionGame &reaction)
    : simon(simon),
      reaction(reaction) {}

void GameManager::startGame(GameType type)
{
    stopGame();

    currentGame = type;

    switch (currentGame)
    {
    case GameType::Simon:
        simon.start();
        break;

    case GameType::Reaction:
        reaction.start();
        break;

    case GameType::None:
        break;
    }
}

void GameManager::stopGame()
{
    switch (currentGame)
    {
    case GameType::Simon:
        simon.stop();
        break;

    case GameType::Reaction:
        reaction.stop();
        break;

    case GameType::None:
        break;
    }

    currentGame = GameType::None;
}

void GameManager::update()
{
    switch (currentGame)
    {
    case GameType::Simon:
        simon.update();
        break;

    case GameType::Reaction:
        reaction.update();
        break;

    case GameType::None:
        break;
    }
}

void GameManager::onTouch(int panel)
{
    switch (currentGame)
    {
    case GameType::Simon:
        simon.onTouch(panel);
        break;

    case GameType::Reaction:
        reaction.onTouch(panel);
        break;

    case GameType::None:
        break;
    }
}

void GameManager::onRelease(int panel)
{
    switch (currentGame)
    {
    case GameType::Simon:
        simon.onRelease(panel);
        break;

    case GameType::Reaction:
        reaction.onRelease(panel);
        break;

    case GameType::None:
        break;
    }
}

GameType GameManager::getCurrentGame() const
{
    return currentGame;
}