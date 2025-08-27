#include "PauseState.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain()
{
    Game::Instance()->getStateMachine()->changeState(new MenuState());
}

void PauseState::s_resumePlay()
{
    Game::Instance()->getStateMachine()->popState();
}

void PauseState::update()
{
    for(int i = 0; (unsigned)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->update();
    }
}

void PauseState::render()
{
    SDL_Rect dummyCamera = {0, 0, 1280, 720}; //setting dummycammera to be able to use sdlgameobject draw
    for (int i = 0; (unsigned)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->draw(dummyCamera);
    }
    Game::Instance()->renderVersionText();
}

bool PauseState::onEnter()
{
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 1280, 720); // this sets render to match camera size
    if(!TextureManager::Instance()->load("res/buttons.png", "resumebutton", Game::Instance()->getRenderer()))
    {
        return false;
    }
    if(!TextureManager::Instance()->load("res/buttons.png", "mainbutton", Game::Instance()->getRenderer()))
    {
        return false;
    }
    if(!TextureManager::Instance()->load("res/background.png", "background", Game::Instance()->getRenderer()))
    {
        std::cerr << "Failed to load background texture!\n";
        return false;
    }
    if(!TextureManager::Instance()->load("res/Laira.png", "title", Game::Instance()->getRenderer()))
    {
        std::cerr << "Failed to load background texture!\n";
        return false;
    }


    GameObject* background = new Background(new LoaderParams(0, 0, 1280, 720, 1, 1, "background"));
    m_gameObjects.push_back(background);
    GameObject* title = new Background(new LoaderParams(529.5f, 200, 221, 46, 1, 1, "title"));
    m_gameObjects.push_back(title);

    GameObject* button1 = new MenuButton(new LoaderParams(592, 268, 96, 32, 1, 4, "mainbutton"), s_pauseToMain);
    GameObject* button2 = new MenuButton(new LoaderParams(592, 300, 96, 32, 1, 3, "resumebutton"), s_resumePlay);

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    std::cout << "Entering PauseState\n";
    return true;
}

bool PauseState::onExit()
{
    // removing objects, clearing texturemaps and setting state to any other 
    for(int i = 0; (unsigned)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->clean();
        delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;
    }
    m_gameObjects.clear();

    TextureManager::Instance()->clearFromTextureMap("resumebutton");
    TextureManager::Instance()->clearFromTextureMap("mainbutton");
    InputHandler::Instance()->reset();
    std::cout << "Exitting PauseState\n";
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 640, 360); // this sets render to match camera size
    return true;
}