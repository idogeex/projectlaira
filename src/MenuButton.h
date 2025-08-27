#pragma once
#include "SDLGameObject.h"

class MenuButton : public SDLGameObject
{
public:
    MenuButton(const LoaderParams* pParams, void (*callback) ());

    virtual void draw(const SDL_Rect& camera) override;
    virtual void update();
    virtual void clean();
    
    bool m_bReleased;
private:
    void (*m_callback) ();

    enum button_state
    {
        MOUSE_OUT = 0,
        MOUSE_OVER = 1,
        CLICKED = 2
    };
};