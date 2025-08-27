#pragma once
#include "SDLGameObject.h"


class Portal : public SDLGameObject
{
public:
    Portal(const LoaderParams* pParams);

    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();
};