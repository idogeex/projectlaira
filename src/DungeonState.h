#pragma once
#include "GameState.h"
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Portal.h"

#include "TileMap.h"
#include "UIManager.h"


enum Direction { UPCOR, DOWNCOR, LEFTCOR, RIGHTCOR };
struct TileData 
{
    int x, y;       // Мировые координаты
    int srcX, srcY; // Позиция в тайлсете
    int tileID;     // ID тайла
};

class DungeonState : public GameState
{
public:
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);

    virtual std::string getStateID() const { return s_dungeonID; }
private:
    static const std::string s_dungeonID;
    std::vector<GameObject*> m_gameObjects;
    SDL_Rect camera;

    TileMap m_collisionMap;
    TileMap* m_tileMap = nullptr;
    UIManager* m_uiManager;
    Player* m_player = nullptr;
    Portal* m_portal = nullptr;
};