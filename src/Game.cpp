#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"

Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL init success\n";
        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(m_pWindow != 0)
        {
            std::cout << "Window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

            if(m_pRenderer != 0)
            {
                std::cout << "Renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
            }
            else
            {
                std::cout << "Renderer init fail\n";
                return false;
            }
            
            if(TTF_Init() == -1)
            {
                std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << "\n";
            }

            m_fontSmall = TTF_OpenFont("res/fonts/Roboto.ttf", 9);
            m_fontMedium = TTF_OpenFont("res/fonts/Roboto.ttf", 24);

            if(!m_fontSmall || !m_fontMedium) 
            {
                std::cerr << "Failed to load fonts: " << TTF_GetError() << std::endl;
                return false;
            }

            m_pGameStateMachine = new GameStateMachine();
            m_pGameStateMachine->changeState(new MenuState());

            InputHandler::Instance()->initialiseJoysticks();
        }
        else
        {
            std::cout << "Window init fail\n";
            return false;
        }
    }
    else
    {
        std::cout << "SDL init fail\n";
        return false;
    }

    std::cout << "init success\n";
    m_bRunning = true;
    return true;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);
    m_pGameStateMachine->render();
    SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
    std::cout << "cleaning up\n";

    if (m_versionTexture) 
    {
        SDL_DestroyTexture(m_versionTexture);
        m_versionTexture = nullptr;
    }

    if (m_fontSmall && m_fontMedium) 
    {
        TTF_CloseFont(m_fontSmall);
        TTF_CloseFont(m_fontMedium);
        m_fontSmall = nullptr;
        m_fontMedium = nullptr;
    }

    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    InputHandler::Instance()->clean();
    TTF_Quit();
    SDL_Quit();
}

void Game::quit()
{
    clean();
    m_bRunning = false;
}

void Game::handleEvents()
{
    InputHandler::Instance()->update();
}

void Game::update()
{
    static Uint32 previousTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    m_pGameStateMachine->update();
}

void Game::renderVersionText() 
{
    if (!m_versionTexture) 
    {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(m_fontMedium, "0.5.0-alpha", {255, 255, 255, 255});
        if (!textSurface) 
        {
            std::cerr << "Failed to create text surface! TTF_Error: " << TTF_GetError() << "\n";
            return;
        }
        m_versionTexture = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);
        SDL_FreeSurface(textSurface);
        int textWidth, textHeight;
        SDL_QueryTexture(m_versionTexture, nullptr, nullptr, &textWidth, &textHeight);
        m_versionRect = {1280 - textWidth - 10, 10, textWidth, textHeight};
    }
    SDL_RenderCopy(m_pRenderer, m_versionTexture, nullptr, &m_versionRect);
}