#pragma once

#include "GameState.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "BlueSlime.h"
#include "Dummy.h"
#include "Portal.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "DungeonState.h"
#include "TileMap.h"

#include "UIManager.h"

class GameObject;

class PlayState : public GameState
{
public:
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();
    bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
    SDL_Rect getCamera() const { return camera; }
    bool isTutorialActive() const override { return m_uiManager->isTutorialActive(); }

    virtual std::string getStateID() const { return s_playID; }
private:
    
    static const std::string s_playID;
    std::vector<GameObject*> m_gameObjects;
    TileMap* m_tileMap = nullptr;
    TileMap m_collisionMap;
    SDL_Rect camera;
    SDL_Point sdlMousePoint;
    Vector2D mousePos;

    UIManager* m_uiManager;
    Enemy* m_selectedEnemy = nullptr;
    Player* m_player = nullptr;
    Portal* m_portal = nullptr;
};