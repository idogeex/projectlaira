#pragma once
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "Game.h"
#include "Vector2D.h"

enum mouse_buttons
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

class InputHandler
{
public:
    static InputHandler* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new InputHandler();
        }
        return s_pInstance;
    }

    void update();
    void clean();
    void reset();

    int xvalue(int joy, int stick);
    int yvalue(int joy, int stick);
    
    void initialiseJoysticks();
    bool joysticksInitialised() { return m_bJoysticksInitialised; }
    bool getButtonState(int joy, int buttonNumber) { return m_buttonStates [joy] [buttonNumber]; }
    bool getMouseButtonState(int buttonNumber) { return m_mouseButtonStates[buttonNumber]; }
    Vector2D* getMousePosition() const { return m_mousePosition; }
    bool isKeyDown(SDL_Scancode key);
    bool isKeyPressed(SDL_Scancode key);

private:
    InputHandler();
    ~InputHandler() {}
    static InputHandler* s_pInstance;
    
    //joystick related stuff
    const int m_joystickDeadZone = 10000;
    std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValue;
    std::vector<SDL_Joystick*> m_joysticks;
    std::vector<std::vector<bool>> m_buttonStates;
    bool m_bJoysticksInitialised;
    
    //mouse related stuff
    std::vector<bool> m_mouseButtonStates;
    Vector2D* m_mousePosition;

    //keyboard related stuff
    const Uint8* m_keystate; // actual keystate
    std::vector<bool> m_prevKeystate; // keystate in previous frame

    //handle keyboard events
    void onKeyDown();
    void onKeyUp();

    //handle mouse events
    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    //handle joysticks events
    void onJoystickAxisMove(SDL_Event& event);
    void onJoystickButtonDown(SDL_Event& event);
    void onJoystickButtonUp(SDL_Event& event);
};