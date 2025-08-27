#include "Enemy.h"

Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams), m_health(50)
{
    m_velocity.setY(0);
    m_velocity.setX(0);
}

void Enemy::draw(const SDL_Rect& camera)
{
    SDLGameObject::draw(camera);
}

void Enemy::update()
{
    SDLGameObject::update();
}

void Enemy::clean()
{

}
