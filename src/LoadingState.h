#pragma once

#include "GameState.h"
#include <string>

class LoadingState : public GameState
{
public:
    LoadingState(GameState* nextState); 
    virtual void update();
    virtual void render();
    virtual bool onEnter();
    virtual bool onExit();
    virtual std::string getStateID() const { return "LOADING"; }

    bool isLoadingComplete() const { return m_loadingComplete; }
    bool isLoadingFailed() const { return m_loadingFailed; }
    GameState* getNextState() const { return m_nextState; }

private:
    GameState* m_nextState; 
    bool m_loadingComplete; 
    bool m_loadingFailed;   
};