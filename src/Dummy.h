#pragma once
#include "Enemy.h"

class Dummy : public Enemy
{
public:
    Dummy(const LoaderParams* pParams);

    std::string getName() const override { return "Dummy"; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    bool isAlive() const override { return m_health > 0; }
    void takeDamage(int damage) override { m_health -= damage; }
    
    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();
private:
    int m_health;
    int m_maxHealth = 500;
};