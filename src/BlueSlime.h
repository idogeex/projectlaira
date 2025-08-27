#pragma once
#include "Enemy.h"

class BlueSlime : public Enemy
{
public:
    BlueSlime(const LoaderParams* pParams);

    std::string getName() const override { return "Blue Slime"; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    bool isAlive() const override { return m_health > 0; }
    void takeDamage(int damage) override { m_health -= damage; }
    int getExpReward() const { 
        std::cout << "Gained exp: " << m_expReward << std::endl;
        return m_expReward; }
    
    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();
private:
    int m_health;
    int m_maxHealth = 10;
};