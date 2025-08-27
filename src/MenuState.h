#pragma once

#include "GameState.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"

class GameObject;

class MenuState : public GameState
{
public:
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return s_menuID; }
private:
    static const std::string s_menuID;
    std::vector<GameObject*> m_gameObjects;
    static void s_menuToPlay();
    static void s_extiFromMenu();
    static void s_menuToOptions();
};

class Background : public SDLGameObject
{
public:
    Background(const LoaderParams* pParams) : SDLGameObject(pParams) {}

    virtual void draw(const SDL_Rect& camera) { SDLGameObject::draw2(camera); }
};