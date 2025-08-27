#include "GameStateMachine.h"
#include "LoadingState.h"

void GameStateMachine::pushState(GameState* pState)
{
    std::cout << "Pushing state: " << pState->getStateID() << std::endl;
    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateMachine::changeState(GameState* pState)
{
    std::cout << "Changing state to: " << pState->getStateID() << std::endl;

    // if current state is targetted - do nothing
    if (!m_gameStates.empty() && m_gameStates.back()->getStateID() == pState->getStateID()) 
    {
        return;
    }

    // if current state is already exists - delete it
    if (!m_gameStates.empty()) 
    {
        if (m_gameStates.back()->onExit()) 
        {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }

    // entering loading state
    LoadingState* loadingState = new LoadingState(pState);
    m_gameStates.push_back(loadingState);
    loadingState->onEnter();
}

void GameStateMachine::popState()
{
    std::cout << "Popping state\n";
    if (!m_gameStates.empty()) 
    {
        if (m_gameStates.back()->onExit()) 
        {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }
}

void GameStateMachine::update()
{
    if (!m_gameStates.empty()) 
    {
        m_gameStates.back()->update();

        // if the current state is loading and loading is completed OR failed
        LoadingState* loadingState = dynamic_cast<LoadingState*>(m_gameStates.back());
        if (loadingState) 
        {
            if (loadingState->isLoadingComplete()) 
            {
                // Entering next states
                GameState* nextState = loadingState->getNextState();
                if (nextState) 
                {
                    std::cout << "Loading complete, entering " << nextState->getStateID() << std::endl;
                    m_gameStates.back()->onExit();
                    m_gameStates.pop_back();
                    m_gameStates.push_back(nextState);
                }
            } 
            else if (loadingState->isLoadingFailed()) 
            {
                std::cout << "Loading failed, returning to previous state\n";
                m_gameStates.back()->onExit();
                m_gameStates.pop_back();
            }
        }
    }
}

void GameStateMachine::render()
{
    if (!m_gameStates.empty()) 
    {
        m_gameStates.back()->render();
    }
}

std::string GameStateMachine::getCurrentStateID()
{
    if (!m_gameStates.empty()) 
    {
        return m_gameStates.back()->getStateID();
    }
    return "";
}