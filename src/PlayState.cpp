#include "PlayState.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
    if(m_uiManager->handleTutorialInput()) 
    {
        return; 
    }
    // changing to PauseState if needed
    if(InputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
    {
        Game::Instance()->getStateMachine()->pushState(new PauseState());
    }

    // update vectors of obj
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->update();
    }

    // making an SDL object of player from GameObject player 
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

    //simple check for interfiering with any of other objects (usually, enemy)
    static Uint32 lastDamageTime = 0;
    const Uint32 damageCooldown = 1000;
    if(InputHandler::Instance()->getMouseButtonState(LEFT))
    {
        mousePos = *InputHandler::Instance()->getMousePosition();
        sdlMousePoint = { (int)mousePos.getX() + camera.x ,(int)mousePos.getY() + camera.y };
        m_selectedEnemy = nullptr;
    }

    for(size_t i = 2; i < m_gameObjects.size(); i++)
    {
        if(auto* enemy = dynamic_cast<Enemy*>(m_gameObjects[i]))
        {
            if(!enemy || !enemy->isAlive()) continue;
            
            SDL_Rect enemyRect = { (int)enemy->getPosition().getX(), (int)enemy->getPosition().getY(), enemy->getWidth(), enemy->getHeight() };
            if(SDL_PointInRect(&sdlMousePoint, &enemyRect))
            {
                m_selectedEnemy = enemy;
                m_player->setTarget(enemy);
                // break;
            }

            if (checkCollision(m_player, enemy)) 
            {
                if (enemy->isAlive()) 
                { 
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastDamageTime > damageCooldown) 
                    {
                        m_player->takeDamage(10);
                        lastDamageTime = currentTime;
                    }
                }
            }

            if(m_selectedEnemy && m_uiManager->handleMouseClick(sdlMousePoint.x, sdlMousePoint.y))
            {
                m_selectedEnemy = nullptr;
            }
        }
    }

    if(m_player->getHP() <= 0)
    {
        Game::Instance()->getStateMachine()->changeState(new GameOverState());
    }

    //deleting enemies and giving exp to player
    auto it = m_gameObjects.begin();
    while (it != m_gameObjects.end()) {
        if (auto* enemy = dynamic_cast<Enemy*>(*it)) {
            if (!enemy->isAlive()) {
                if (m_selectedEnemy == enemy) {
                    m_player->gainEXP(m_selectedEnemy->getExpReward());
                    m_selectedEnemy = nullptr;
                    m_player->setTarget(nullptr);
                }
                delete *it;
                it = m_gameObjects.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    if(checkCollision(m_player, m_portal) && m_player->used)
    {
        Game::Instance()->getStateMachine()->pushState(new DungeonState());
    }
}

// simply renders all with camera as a parameter 
void PlayState::render()
{
    if(m_tileMap)
    {
        m_tileMap->draw(Game::Instance()->getRenderer(), camera, "upperworld", 32, 32);
    }

    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        if (auto* obj = dynamic_cast<SDLGameObject*>(m_gameObjects[i])) {
            if (obj->isVisible()) 
            {
                obj->draw(camera);
            }
        }
    }
    m_uiManager->renderPlayerStatus(*m_player, camera);
    m_uiManager->renderEnemyWindow(m_selectedEnemy, camera);
    m_uiManager->renderTutorialWindow(camera);
}

bool PlayState::onEnter()
{
    camera = {0, 0, 640, 360};
    SDL_RenderSetLogicalSize(Game::Instance()->getRenderer(), 640, 360);

    if(!TextureManager::Instance()->load("res/cheliks2.png", "player", Game::Instance()->getRenderer()))
    { 
        std::cout << "Failed to load player texture\n";
        return false;
    }

    if(!TextureManager::Instance()->load("res/Blue Slime.png", "slime", Game::Instance()->getRenderer()))
    {
        std::cout << "Failed to load slime texture\n";
        return false;
    }

    if(!TextureManager::Instance()->load("res/dummy.png", "dummy", Game::Instance()->getRenderer()))
    {
        std::cout << "Failed to load dummy texture\n";
        return false;
    }

    if(!TextureManager::Instance()->load("res/upperworld.png", "upperworld", Game::Instance()->getRenderer()))
    {
        std::cout << "Failed to load upperworld texture\n";
        return false;
    }

    if(!TextureManager::Instance()->load("res/portal.png", "portal", Game::Instance()->getRenderer())) return false;

    m_tileMap = new TileMap();
    if(!m_tileMap->loadFromCSV("res/upperworld_map.csv"))
    {
        std::cout << "Failed to load map\n";
        delete m_tileMap;
        m_tileMap = nullptr;
        return false;
    }

    if(!m_collisionMap.loadFromCSV2("res/collision_map.csv"))
    {
        std::cout << "Failed to load collision map\n";
        return false;
    }

    m_portal = new Portal(new LoaderParams(400, 1336, 32, 32, 1, 1, "portal"));
    m_gameObjects.push_back(m_portal);

    GameObject* player = new Player(new LoaderParams(288, 1336, 32, 32, 1, 6, "player"), m_collisionMap);
    m_gameObjects.push_back(player);
    m_player = dynamic_cast<Player*>(m_gameObjects[1]);

    GameObject* blue_slime = new BlueSlime(new LoaderParams(600, 1240, 32, 32, 1, 1, "slime"));
    m_gameObjects.push_back(blue_slime);
    GameObject* blue_slime2 = new BlueSlime(new LoaderParams(696, 1240, 32, 32, 1, 1, "slime"));
    m_gameObjects.push_back(blue_slime2);
    GameObject* blue_slime4 = new BlueSlime(new LoaderParams(796, 1240, 32, 32, 1, 1, "slime"));
    m_gameObjects.push_back(blue_slime4);
    GameObject* blue_slime5 = new BlueSlime(new LoaderParams(896, 1240, 32, 32, 1, 1, "slime"));
    m_gameObjects.push_back(blue_slime5);
    GameObject* blue_slime3 = new BlueSlime(new LoaderParams(996, 1240, 32, 32, 1, 1, "slime"));
    m_gameObjects.push_back(blue_slime3);

    GameObject* dummy = new Dummy(new LoaderParams(300, 1240, 32, 32, 1, 1, "dummy"));
    m_gameObjects.push_back(dummy);
    GameObject* dummy2 = new Dummy(new LoaderParams(332, 1240, 32, 32, 1, 1, "dummy"));
    m_gameObjects.push_back(dummy2);

    m_uiManager = new UIManager(Game::Instance()->getRenderer());
    m_uiManager->setTutorialActive(true);
    m_uiManager->setTutorialText({"Controls:", "WASD/Arrows - Move", "F1 - P. Attack", "F2 - M. Attack", "LMC - Set a target", "ESC - Pause", "E - Interact with the Portal", "Hold CTRL for noclip" });

    return true;
}

// removing objects, clearing texturemaps and setting state to any other 
bool PlayState::onExit()
{
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->clean();
        delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;
    }
    m_gameObjects.clear();

    if(m_tileMap)
    {
        delete m_tileMap;
        m_tileMap = nullptr;
    }
    TextureManager::Instance()->clearFromTextureMap("player");
    TextureManager::Instance()->clearFromTextureMap("slime");
    TextureManager::Instance()->clearFromTextureMap("dummy");
    TextureManager::Instance()->clearFromTextureMap("upperworld");
    std::cout<< "exitting playstate\n";
    return true;
}

// simple collision logic for checking if collided with slime 
bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
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