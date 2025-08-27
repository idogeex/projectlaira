#include "Portal.h"

Portal::Portal(const LoaderParams *pParams) : SDLGameObject(pParams)
{

}

void Portal::draw(const SDL_Rect& camera)
{
    SDLGameObject::drawShadow(camera, 4, 15, 29);
    SDLGameObject::draw(camera);
}

void Portal::update()
{
    m_currentFrame = int(((SDL_GetTicks() / 100) % 1));
    SDLGameObject::update();
}

void Portal::clean()
{
    //nothing to do atm
}