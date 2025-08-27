#include "GameOverState.h"
#include <iostream>


const std::string GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::s_gameOverToMain()
{
    Game::Instance()->getStateMachine()->changeState(new MenuState());
    SDL_Delay(100);
}

void GameOverState::s_restartPlay()
{
    Game::Instance()->getStateMachine()->changeState(new PlayState());
    SDL_Delay(100);
}

bool GameOverState::onEnter()
{
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 1280, 720); // this sets render to match camera size
    if(!TextureManager::Instance()->load("res/buttons.png", "mainbutton", Game::Instance()->getRenderer()))
    {
        std::cout << "failed to load texture\n";
        return false;
    }
    if(!TextureManager::Instance()->load("res/buttons.png", "restartbutton", Game::Instance()->getRenderer()))
    {
        std::cout << "failed to load texture\n";
        return false;
    }
    if(!TextureManager::Instance()->load("res/background.png", "background", Game::Instance()->getRenderer()))
    {
        std::cout << "failed to load texture\n";
        return false;
    }
    GameObject* background = new Background(new LoaderParams(0, 0, 1280, 720, 1, 1, "background"));
    GameObject* button1 = new MenuButton(new LoaderParams(580, 268, 96, 32, 1, 4, "mainbutton"), s_gameOverToMain);
    GameObject* button2 = new MenuButton(new LoaderParams(580, 300, 96, 32, 1, 5, "restartbutton"), s_restartPlay);
    m_gameObjects.push_back(background);
    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    std::cout << "entering GameOverState\n";
    return true;
}

bool GameOverState::onExit()
{
    // removing objects, clearing texturemaps and setting state to any other 
    for(int i = 0; (unsigned int)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->clean();
        delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;
    }
    m_gameObjects.clear();
    TextureManager::Instance()->clearFromTextureMap("mainbutton");
    TextureManager::Instance()->clearFromTextureMap("restartbutton");
    std::cout << "exitting GameOverState\n";
    return true;
}

void GameOverState::update()
{
    for(int i = 0; (unsigned int)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->update();
    }
}

void GameOverState::render()
{
    SDL_Rect dummyCamera = {0, 0, 1280, 720}; //setting dummycamera to be able to use sdlgameobject draw
    for (int i = 0; (unsigned)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->draw(dummyCamera);
    }
    Game::Instance()->renderVersionText();
}