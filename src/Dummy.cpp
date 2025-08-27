#include "Dummy.h"

Dummy::Dummy(const LoaderParams* pParams) : Enemy(pParams)
{
    m_health = 500;
    m_expReward = 25;
    m_velocity.setY(0);
    m_velocity.setX(0);
}

void Dummy::draw(const SDL_Rect& camera)
{
    SDLGameObject::drawTitle(camera, "Dummy", 5);
    SDLGameObject::drawShadow(camera, 4, 15, 23);
    SDLGameObject::draw(camera);
}

void Dummy::update()
{

}

void Dummy::clean()
{

}