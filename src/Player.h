#pragma once

#include "SDLGameObject.h"
#include "InputHandler.h"
#include "TileMap.h"
#include "Enemy.h"

class Player : public SDLGameObject
{
public:
    Player(const LoaderParams* pParams, const TileMap& collisionMap);

    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();

    //status
    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_maxHP; }
    int getMP() const { return m_mp; }
    int getMaxMP() const { return m_maxMP; }
    float getEXPPercent() const { return (m_exp / m_expToNextLevel) * 100; }

    //game logic
    void checkCollision();
    void takeDamage(int damage) { m_hp = std::max(0, m_hp - damage); }
    void useMana(int amount) { m_mp = std::max(0, m_mp - amount); }
    void setTarget(Enemy* target) { m_target = target; }
    void gainEXP(int amount);

    bool used = false;

private:
    void handleInput();
    float maxSpeed = 1.0f;
    const TileMap& m_collisionMap;

    Enemy* m_target = nullptr;

    //status
    int m_hp = 100;
    int m_maxHP = 100;
    int m_mp = 50;
    int m_maxMP = 50;
    float m_exp = 0;
    float m_expToNextLevel = 100;
    int currentlevel = 1;

    int pAttack;
    int mAttack;
};