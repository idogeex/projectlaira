#include "MenuState.h"

const std::string MenuState::s_menuID = "MENU";

void MenuState::update()
{
    for(int i = 0; (unsigned int)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->update();
    }
}

void MenuState::render()
{
    SDL_Rect dummyCamera = {0, 0, 1280, 720}; //setting dummycamera, to be able to use sdlgameobject draw
    for (size_t i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->draw(dummyCamera);
    }
    Game::Instance()->renderVersionText();
}

bool MenuState::onEnter()
{
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 1280, 720); // this sets render to match camera size
    if(!TextureManager::Instance()->load("res/buttons.png", "playbutton", Game::Instance()->getRenderer()))
    {
        return false;
    }

    if(!TextureManager::Instance()->load("res/buttons.png", "exitbutton", Game::Instance()->getRenderer()))
    {
        return false;
    }

    if(!TextureManager::Instance()->load("res/buttons.png", "options", Game::Instance()->getRenderer()))
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

    GameObject* button1 = new MenuButton(new LoaderParams(592, 300, 96, 32, 1, 1, "playbutton"), s_menuToPlay);
    GameObject* button2 = new MenuButton(new LoaderParams(592, 364, 96, 32, 1, 2, "exitbutton"), s_extiFromMenu);
    GameObject* button3 = new MenuButton(new LoaderParams(592, 332, 96, 32, 1, 6, "options"), s_menuToOptions);


    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    m_gameObjects.push_back(button3);
    std::cout << "entering menustate\n";
    return true;

}

bool MenuState::onExit()
{
    // removing objects, clearing texturemaps and setting state to any other 
    for(int i = 0; (unsigned int)i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->clean();
        delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;
    }
    m_gameObjects.clear();
    TextureManager::Instance()->clearFromTextureMap("playbutton");
    TextureManager::Instance()->clearFromTextureMap("exitbutton");
    TextureManager::Instance()->clearFromTextureMap("options");
    TextureManager::Instance()->clearFromTextureMap("background");
    TextureManager::Instance()->clearFromTextureMap("title");
    std::cout << "exitting menustate\n";
    return true;
}

void MenuState::s_menuToPlay()
{
    Game::Instance()->getStateMachine()->changeState(new PlayState());
}

void MenuState::s_extiFromMenu()
{
    Game::Instance()->quit();
}

void MenuState::s_menuToOptions()
{

}