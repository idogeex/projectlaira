#include "BlueSlime.h"

BlueSlime::BlueSlime(const LoaderParams* pParams) : Enemy(pParams)
{
    m_health = 10;
    m_expReward = 25;
    m_velocity.setY(0);
    m_velocity.setX(0);
}

void BlueSlime::draw(const SDL_Rect& camera)
{
    SDLGameObject::drawTitle(camera, "Blue Slime", 5);
    SDLGameObject::drawShadow(camera, 4, 15, 23);
    SDLGameObject::draw(camera);
}

void BlueSlime::update()
{
    m_currentFrame = int(((SDL_GetTicks() / 100) % 1));
    if(m_position.getX() == 896 && m_position.getY() == 288)
    {
        m_velocity.setX(1);
        m_velocity.setY(0);
    }
    else if(m_position.getX() == 1152 && m_position.getY() == 288)
    {
        m_velocity.setX(0);
        m_velocity.setY(1);
    }
    else if(m_position.getX() == 1152 && m_position.getY() == 480)
    {
        m_velocity.setX(-1);
        m_velocity.setY(0);
    }
    else if(m_position.getX() == 896 && m_position.getY() == 480)
    {
        m_velocity.setX(0);
        m_velocity.setY(-1);
    }
    SDLGameObject::update();
}

void BlueSlime::clean()
{

}
