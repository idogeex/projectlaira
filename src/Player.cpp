#include "Player.h"
#include "UIManager.h"

Player::Player(const LoaderParams* pParams, const TileMap& collisionMap) : SDLGameObject(pParams), m_collisionMap(collisionMap)
{

}

void Player::handleInput()
{  
    if (Game::Instance()->getStateMachine()->getCurrentState()->isTutorialActive()) 
    {
        m_velocity.setX(0);
        m_velocity.setY(0);
        return;
    }

    //resetting velocities
    m_velocity.setX(0);
    m_velocity.setY(0);
    
    //some bools for more readability
    bool movingRight = InputHandler::Instance()->isKeyDown(SDL_SCANCODE_D) || InputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT);
    bool movingLeft = InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A) || InputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT);
    bool movingUp = InputHandler::Instance()->isKeyDown(SDL_SCANCODE_W) || InputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP);
    bool movingDown = InputHandler::Instance()->isKeyDown(SDL_SCANCODE_S) || InputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN);

    //movement logic
    if (movingRight && movingDown)
    {
        m_velocity.setX(maxSpeed);
        m_velocity.setY(maxSpeed);
        m_currentRow = 2;
    }
    else if (movingLeft && movingDown)
    {
        m_velocity.setX(-maxSpeed);
        m_velocity.setY(maxSpeed);
        m_currentRow = 2;
    }
    else if (movingRight && movingUp)
    {
        m_velocity.setX(maxSpeed);
        m_velocity.setY(-maxSpeed);
        m_currentRow = 4;
    }
    else if (movingLeft && movingUp)
    {
        m_velocity.setX(-maxSpeed);
        m_velocity.setY(-maxSpeed);
        m_currentRow = 4;
    }
    else if (movingRight)
    {
        m_velocity.setX(maxSpeed);
        m_currentRow = 3;
    }
    else if (movingLeft)
    {
        m_velocity.setX(-maxSpeed);
        m_currentRow = 3;
    }
    else if (movingUp)
    {
        m_velocity.setY(-maxSpeed);
        m_currentRow = 5;
    }
    else if (movingDown)
    {
        m_velocity.setY(maxSpeed);
        m_currentRow = 1;
    }
    else
    {
        m_currentRow = 6; //idle
    }

    //normalaize player velocity 
    if (m_velocity.getX() != 0 && m_velocity.getY() != 0)
    {
        m_velocity.setX(maxSpeed * m_velocity.getX() / 1.4142f);
        m_velocity.setY(maxSpeed * m_velocity.getY() / 1.4142f);
    }
    
    if(InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_1)) std::cout << "X pos is " << m_position.getX() << " Y pos is " << m_position.getY() << std::endl;

    if(InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_3)) Player::gainEXP(10);

    if(InputHandler::Instance()->isKeyDown(SDL_SCANCODE_LCTRL)) maxSpeed = 3;
    else maxSpeed = 1;

    if(InputHandler::Instance()->isKeyDown(SDL_SCANCODE_E)) used = true;
    else used = false;

    if (InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_F1)) 
    {
        if (m_target && m_target->isAlive())
         {
            Vector2D playerCenter = m_position + Vector2D(m_width / 2, m_height / 2);
            Vector2D enemyCenter = m_target->getPosition() + Vector2D(m_target->getWidth() / 2, m_target->getHeight() / 2);
            float distance = playerCenter.distance(enemyCenter);
            
            if (distance <= 30.0f) 
            {
                m_target->takeDamage(pAttack);
                std::cout << "pAttack is " << pAttack << std::endl;
                if (!m_target->isAlive()) 
                {
                    m_target = nullptr;
                }
            }
        }
    }

    if (InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_F2) && m_mp >= 5) 
    {
        if (m_target && m_target->isAlive())
         {
            Vector2D playerCenter = m_position + Vector2D(m_width / 2, m_height / 2);
            Vector2D enemyCenter = m_target->getPosition() + Vector2D(m_target->getWidth() / 2, m_target->getHeight() / 2);
            float distance = playerCenter.distance(enemyCenter);
            
            if (distance >= 30.0f) 
            {
                m_target->takeDamage(mAttack);
                std::cout << "mAttack is " << mAttack << std::endl;
                if (!m_target->isAlive()) 
                {
                    m_target = nullptr;
                }
                m_mp = m_mp - 5;
            }
        }
    }

    if(InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_2)) std::cout << "Player X velocity is " << m_velocity.getX() << " Player Y velocity is " << m_velocity.getY() << std::endl;
}

void Player::draw(const SDL_Rect& camera)
{
    SDLGameObject::drawShadow(camera, 4, 15, 29);
    SDLGameObject::draw(camera);
}

void Player::update()
{
    m_currentFrame = int(((SDL_GetTicks() / 100) % 6));

    handleInput();

    SDLGameObject::update();

    checkCollision();

    pAttack = ((std::rand() % 10) + (currentlevel));
    mAttack = ((std::rand() % 7) + (currentlevel));
}

// player collision with tiles
void Player::checkCollision()
{
    float playerX1 = (m_position.getX() + 12) / 32.0; // left up
    float playerY1 = (m_position.getY() + 16) / 32.0;
    float playerX2 = (m_position.getX() + m_width - 14) / 32.0; // right up
    float playerY2 = (m_position.getY() + 16) / 32.0;
    float playerX3 = (m_position.getX() + 12) / 32.0; // left bottom
    float playerY3 = (m_position.getY() + m_height - 1) / 32.0;
    float playerX4 = (m_position.getX() + m_width - 14) / 32.0; // right bottom
    float playerY4 = (m_position.getY() + m_height - 1) / 32.0;

    bool collision1 = m_collisionMap.isCollision(playerX1, playerY1); //player left up corner
    bool collision2 = m_collisionMap.isCollision(playerX2, playerY2); //player right up corner
    bool collision3 = m_collisionMap.isCollision(playerX3, playerY3); //player left bottom corner
    bool collision4 = m_collisionMap.isCollision(playerX4, playerY4); //player right bottom corner

    //checking for collision by Y axis
    if(m_velocity.getY() != 0)
    {
        if(m_velocity.getY() > 0) // moving down
        {
            if(collision3 || collision4)
            {
                m_velocity.setY(1);
                m_position.setY(m_position.getY() - 1);
            }
        }
        else if(m_velocity.getY() < 0) // moving upwards
        {
            if(collision1 || collision2)
            {
                m_velocity.setY(-1);
                m_position.setY(m_position.getY() + 1);
            }
        }
    }

    //checking for collision by X axis
    if(m_velocity.getX() != 0)
    {
        if(m_velocity.getX() > 0) // moving right
        {
            if(collision2 || collision4)
            {
                m_velocity.setX(1);
                m_position.setX(m_position.getX() - 1.0f);
            }
        }
        else if(m_velocity.getX() < 0) // moving left
        {
            if(collision1 || collision3)
            {
                m_velocity.setX(-1);
                m_position.setX(m_position.getX() + 1.0f);
            }
        }
    }

    // preventing player to exceed world size
    int worldWidth = Game::Instance()->getWorldWidth();
    int worldHeight = Game::Instance()->getWorldHeight();

    if (m_position.getX() < 0)
    {
        m_position.setX(0);
    }
    if (m_position.getY() < 0)
    {
        m_position.setY(0);
    }
    if (m_position.getX() + m_width > worldWidth)
    {
        m_position.setX(worldWidth - m_width);
    }
    if (m_position.getY() + m_height > worldHeight)
    {
        m_position.setY(worldHeight - m_height);
    }
}

void Player::clean()
{
    SDLGameObject::clean();
}

void Player::gainEXP(int amount)
{ 
    m_exp += amount; 
    if (m_exp >= m_expToNextLevel) 
    {
        currentlevel++;
        m_exp -= m_expToNextLevel;
        m_expToNextLevel *= 1.5f;
    }
}