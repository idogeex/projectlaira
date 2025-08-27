#pragma once
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "TextureManager.h"
#include "GameObject.h"
#include "SDLGameObject.h"
#include "InputHandler.h"
#include "GameStateMachine.h"

class Game
{
public:

    static Game* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new Game();
            return s_pInstance;
        }

        return s_pInstance;
    }


    bool init(const char* title, int xpos, int ypos, int height, int width, int flags);
    void render();
    void update();
    void handleEvents();
    void clean();
    void quit();

    bool running() const { return m_bRunning; }

    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    GameStateMachine* getStateMachine() const { return m_pGameStateMachine; }

    int getWorldWidth() const { return m_worldWidth; }
    int getWorldHeight() const { return m_worldHeight; }
    float getDeltaTime() const { return m_deltaTime; }
    TTF_Font* getFontSmall() const { return m_fontSmall; }
    TTF_Font* getFontMedium() const { return m_fontMedium; }

    void renderVersionText();

private:
    Game() {};
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    int m_currentFrame;

    int m_worldWidth = 100 * 32;
    int m_worldHeight = 50 * 32;

    float m_deltaTime = 0.0f;

    GameStateMachine* m_pGameStateMachine;

    std::vector<GameObject*> m_gameObjects;

    bool m_bRunning;
    static Game* s_pInstance;

    TTF_Font* m_fontSmall;
    TTF_Font* m_fontMedium;
    SDL_Texture* m_versionTexture;
    SDL_Rect m_versionRect;
}; 