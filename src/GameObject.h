#pragma once
#include <iostream>

#include <SDL2/SDL.h>

#include "LoaderParams.h"

class GameObject
{
public:
    virtual ~GameObject() noexcept {}
    virtual void draw(const SDL_Rect& camera) = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
    int getZIndex() const { return m_zIndex; }
protected:
    GameObject(const LoaderParams* pParams) {}
    int m_zIndex;
};