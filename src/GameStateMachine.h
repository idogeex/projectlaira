#pragma once

#include <vector>

#include "GameState.h"

class GameStateMachine
{
public:
    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();

    std::string getCurrentStateID();
    void update();
    void render();

    GameState* getCurrentState() const { return m_gameStates.back(); }

private:
    std::vector<GameState*> m_gameStates;
};