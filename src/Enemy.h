#pragma once

#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
public:
    Enemy(const LoaderParams* pParams);

    virtual std::string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual bool isAlive() const { return m_health > 0; }
    virtual void takeDamage(int damage) {m_health -= damage; if (!isAlive()) { setVisible(false); } }
    virtual int getHP() const { return m_health; }
    virtual int getExpReward() const { 
        std::cout << "Gained exp: " << m_expReward << std::endl;
        return m_expReward; }


    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();
protected:
    int m_health;
    int m_expReward = 25;
};