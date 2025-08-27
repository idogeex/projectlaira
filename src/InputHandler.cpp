#include "InputHandler.h"

InputHandler* InputHandler::s_pInstance = 0;

InputHandler::InputHandler() : m_mousePosition(new Vector2D(0, 0))
{
    for(int i = 0; i < 3; i++)
    {
        m_mouseButtonStates.push_back(false);
    }
    m_prevKeystate.resize(SDL_NUM_SCANCODES, false);
    m_keystate = SDL_GetKeyboardState(nullptr);
}

void InputHandler::initialiseJoysticks()
{
    if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
    {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    if(SDL_NumJoysticks() > 0)
    {
        for(int i = 0; i < SDL_NumJoysticks(); i++)
        {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if(SDL_JoystickOpen(i))
            {
                m_joysticks.push_back(joy);
                m_joystickValue.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

                std::vector<bool> tempButtons;
                for(int j = 0; j< SDL_JoystickNumButtons(joy); j++)
                {
                    tempButtons.push_back(false);
                }
                m_buttonStates.push_back(tempButtons);
            }
            else
            {
                std::cout << SDL_GetError();
            }
        }
        SDL_JoystickEventState(SDL_ENABLE);
        m_bJoysticksInitialised = true;

        std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
    }
    else
    {
        m_bJoysticksInitialised = false;
    }
}

int InputHandler::xvalue(int joy, int stick)
{
    if(m_joystickValue.size() > 0)
    {
        if(stick == 1)
        {
            return m_joystickValue[joy].first->getX();
        }
        else if(stick == 2)
        {
            return m_joystickValue[joy].second->getX();
        }
    }
    return 0;
}

int InputHandler::yvalue(int joy, int stick)
{
    if(m_joystickValue.size() > 0)
    {
        if(stick == 1)
        {
            return m_joystickValue[joy].first->getY();
        }
        else if (stick == 2)
        {
            return m_joystickValue[joy].second->getY();
        }
    }
    return 0;
}

void InputHandler::clean()
{
    if(m_bJoysticksInitialised)
    {
        for(int i = 0; i < SDL_NumJoysticks(); i++)
        {
            SDL_JoystickClose(m_joysticks[i]);
        }
    }
}

void InputHandler::update()
{
    if(m_keystate != nullptr)
    {
        for(int i = 0; i < SDL_NUM_SCANCODES; i++)
        {
            m_prevKeystate[i] = m_keystate[i];
        }
    }

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Game::Instance()->quit();
        break;

        case SDL_JOYAXISMOTION:
            onJoystickAxisMove(event);
        break;

        case SDL_JOYBUTTONDOWN:
            onJoystickButtonDown(event);
        break;

        case SDL_JOYBUTTONUP:
            onJoystickButtonUp(event);
        break;

        case SDL_MOUSEMOTION:
            onMouseMove(event);
        break;

        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
        break;

        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
        break;

        case SDL_KEYDOWN:
            onKeyDown();
        break;

        case SDL_KEYUP:
            onKeyUp();
        break;

        default:
        break;
        }
    }

    m_keystate = SDL_GetKeyboardState(nullptr);
}

void InputHandler::onKeyDown()
{
    m_keystate = SDL_GetKeyboardState(nullptr);
}

void InputHandler::onKeyUp()
{
    m_keystate = SDL_GetKeyboardState(nullptr);
}

void InputHandler::onJoystickButtonUp(SDL_Event& event)
{
    if(event.type == SDL_JOYBUTTONUP)
    {
        int whichOne = event.jaxis.which;
        m_buttonStates[whichOne] [event.jbutton.button] = false;
    }
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)
{
    if(event.type == SDL_JOYBUTTONDOWN)
    {
        int whichOne = event.jaxis.which;
        m_buttonStates[whichOne] [event.jbutton.button] = true;
    }
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
    if(event.type == SDL_JOYAXISMOTION)
    {
        int whichOne = event.jaxis.which;
        if(event.jaxis.axis == 0)
        {
            if(event.jaxis.value > m_joystickDeadZone)
            {
                m_joystickValue[whichOne].first->setX(1);
            }
            else if(event.jaxis.value < -m_joystickDeadZone)
            {
                m_joystickValue[whichOne].first->setX(-1);
            }
            else
            {
                m_joystickValue[whichOne].first->setX(0);
            }
        }

        if(event.jaxis.axis == 1)
        {
            if(event.jaxis.value > m_joystickDeadZone)
            {
                m_joystickValue[whichOne].first->setY(1);
            }
            else if(event.jaxis.value < -m_joystickDeadZone)
            {
                m_joystickValue[whichOne].first->setY(-1);
            }
            else
            {
                m_joystickValue[whichOne].first->setY(0);
            }
        }

        if(event.jaxis.axis == 2)
        {
            if(event.jaxis.value > m_joystickDeadZone)
            {
                m_joystickValue[whichOne].second->setX(1);
            }
            else if(event.jaxis.value < -m_joystickDeadZone)
               {
                m_joystickValue[whichOne].second->setX(-1);
            }
            else
            {
                m_joystickValue[whichOne].second->setX(0);
            }
        }

        if(event.jaxis.axis == 3)
        {
            if(event.jaxis.value > m_joystickDeadZone)
            {
                m_joystickValue[whichOne].second->setY(1);
            }
            else if(event.jaxis.value < -m_joystickDeadZone)
            {
                m_joystickValue[whichOne].second->setY(-1);
            }
            else
            {
                m_joystickValue[whichOne].second->setY(0);
            }
        }
    }
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
             m_mouseButtonStates[LEFT] = true;
        }

        if(event.button.button == SDL_BUTTON_MIDDLE)
            {
                 m_mouseButtonStates[MIDDLE] = true;
            }

        if(event.button.button == SDL_BUTTON_RIGHT) 
        {
            m_mouseButtonStates[RIGHT] = true;
        }  
    }    
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
    if(event.type == SDL_MOUSEBUTTONUP)
    {
       if(event.button.button == SDL_BUTTON_LEFT)
        {
            m_mouseButtonStates[LEFT] = false;
        }

        if(event.button.button == SDL_BUTTON_MIDDLE)
        {
            m_mouseButtonStates[MIDDLE] = false;
        }

        if(event.button.button == SDL_BUTTON_RIGHT)
        {
            m_mouseButtonStates[RIGHT] = false;
        }
    }
}

void InputHandler::onMouseMove(SDL_Event& event)
{
    if(event.type == SDL_MOUSEMOTION)
    {
        m_mousePosition->setX(event.motion.x);
        m_mousePosition->setY(event.motion.y);
    }
}

bool InputHandler::isKeyDown(SDL_Scancode key)

{
    if(m_keystate != nullptr)
    {
        return m_keystate[key] == 1;
    }
    return false;
}

void InputHandler::reset()
{
    m_mouseButtonStates[LEFT] = false;
    m_mouseButtonStates[RIGHT] = false;
    m_mouseButtonStates[MIDDLE] = false;
}

bool InputHandler::isKeyPressed(SDL_Scancode key)
{
    if(m_keystate != nullptr)
    {
        return m_keystate[key] && !m_prevKeystate[key];
    }
    return false;
}