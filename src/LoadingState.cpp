#include "LoadingState.h"
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"

LoadingState::LoadingState(GameState* nextState)
    : m_nextState(nextState), m_loadingComplete(false), m_loadingFailed(false)
{
}

void LoadingState::update()
{
    
    if (m_loadingComplete || m_loadingFailed) 
    {
        return;
    }

    if (m_nextState->onEnter()) 
    {
        m_loadingComplete = true;
    } 
    else 
    {
        std::cout << "Failed to load next state\n";
        m_loadingFailed = true;
    }
}

void LoadingState::render()
{
    SDL_RenderClear(Game::Instance()->getRenderer());

    if (m_loadingFailed) 
    {
        std::cout << "Rendering error message: Failed to load resources\n";
    } 
    else 
    {

    }
    SDL_RenderPresent(Game::Instance()->getRenderer());
}

bool LoadingState::onEnter()
{
    std::cout << "Entering LoadingState\n";
    return true;
}

bool LoadingState::onExit()
{
    std::cout << "Exiting LoadingState\n";

    if (m_loadingFailed) 
    {
        delete m_nextState;
        m_nextState = nullptr;
    }

    return true;
}