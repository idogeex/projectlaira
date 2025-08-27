#include "DungeonState.h"
#include "PauseState.h"
#include <random>
#include <algorithm>

const std::string DungeonState::s_dungeonID = "DUNGEON";

void DungeonState::update()
{
    if(InputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
    {
        Game::Instance()->getStateMachine()->pushState(new PauseState());
    }

    for(auto obj : m_gameObjects)
    {
        obj->update();
    }

    if (m_player) // if player is initialised, then do camera logic
    {
        // making camera centers player
        camera.x = m_player->getPosition().getX() + m_player->getWidth() / 2 - camera.w / 2;
        camera.y = m_player->getPosition().getY() + m_player->getHeight() / 2 - camera.h / 2;

        // real world size 
        int worldWidth = Game::Instance()->getWorldWidth();
        int worldHeight = Game::Instance()->getWorldHeight(); 

        // checking if camera exceeds world size and blocking it to do so
        if (camera.x < 0)
        { 
            camera.x = 0;
        }
        if (camera.y < 0) 
        {
            camera.y = 0;
        }
        if (camera.x > worldWidth - camera.w) 
        {
            camera.x = worldWidth - camera.w;
        }
        if (camera.y > worldHeight - camera.h) 
        {
            camera.y = worldHeight - camera.h;
        }
    }

    if(checkCollision(m_player, m_portal) && m_player->used)
    {
        Game::Instance()->getStateMachine()->popState();
        SDL_Delay(100);
    }

}

void DungeonState::render()
{
    if(m_tileMap)
    {
        m_tileMap->draw2(Game::Instance()->getRenderer(), camera, "dungeon", 32, 32);
    }

    for (auto obj : m_gameObjects) {
        obj->draw(camera);
    }
    m_uiManager->renderPlayerStatus(*m_player, camera);
}

bool DungeonState::onEnter()
{
    camera = {0, 0, 640, 360};
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 640, 360);
    if(!TextureManager::Instance()->load("res/cheliks2.png", "player", Game::Instance()->getRenderer()))
    { 
        std::cout << "Failed to load player texture\n";
        return false;
    }

    if(!TextureManager::Instance()->load("res/dungeontiles.png", "dungeon", Game::Instance()->getRenderer())) return false;

    m_tileMap = new TileMap();
    if(!m_tileMap->loadFromCSV("res/dungeonmap.csv"))
    {
        std::cout << "Failed to load map\n";
        delete m_tileMap;
        m_tileMap = nullptr;
        return false;
    }

    if(!m_collisionMap.loadFromCSV2("res/dungeonmap_collision.csv"))
    {
        std::cout << "Failed to load collision map\n";
        return false;
    }

    m_portal = new Portal(new LoaderParams(841, 649, 32, 32, 1, 1, "portal"));
    m_gameObjects.push_back(m_portal);

    GameObject* player = new Player(new LoaderParams(1340, 700, 32, 32, 1, 6, "player"), m_collisionMap);
    m_gameObjects.push_back(player);
    
    m_player = dynamic_cast<Player*>(m_gameObjects[1]);
    m_uiManager = new UIManager(Game::Instance()->getRenderer());

    return true;
}

bool DungeonState::onExit()
{
    for(auto obj : m_gameObjects)
    {
        delete obj;
    }
    m_gameObjects.clear();
    return true;
}

bool DungeonState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = p1->getPosition().getX();
    rightA = p1->getPosition().getX() + p1->getWidth();
    topA = p1->getPosition().getY();
    bottomA = p1->getPosition().getY() + p1->getHeight();

    leftB = p2->getPosition().getX();
    rightB = p2->getPosition().getX() + p2->getWidth();
    topB = p2->getPosition().getY();
    bottomB = p2->getPosition().getY() + p2->getHeight();

    if( bottomA <= topB ) { return false; }
    if( topA >= bottomB ) { return false; }
    if( rightA <= leftB ) { return false; }
    if( leftA >= rightB ) { return false; }
    
    return true;
}