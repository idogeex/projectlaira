#pragma once
#include <iostream>
#include <string>
#include <vector>

class GameState
{
public:
    virtual void update() = 0;
    virtual void render() = 0;
    
    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    virtual bool isTutorialActive() const { return false; }

    virtual std::string getStateID() const = 0;
    virtual ~GameState() = default;
};