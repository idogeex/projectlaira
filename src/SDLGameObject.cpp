#include "SDLGameObject.h"
#include "PlayState.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) : GameObject(pParams), m_position(pParams->getX(), pParams->getY()), m_velocity(0, 0)
{
    m_width = pParams->getWidth();
    m_height = pParams->getHeight();
    m_textureID = pParams->getTextureID();

    m_currentRow = pParams->getCurrentRow();
    m_currentFrame = pParams->getCurrentFrame();
}

//drawing any objects with camera reference
void SDLGameObject::draw(const SDL_Rect& camera)
{
    SDL_Rect dest = { (int)(m_position.getX() - camera.x), (int)(m_position.getY() - camera.y), m_width, m_height };

    if (m_velocity.getX() < 0)
    {
        TextureManager::Instance()->drawFrame(m_textureID, dest.x, dest.y, m_width, m_height, m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), SDL_FLIP_HORIZONTAL
        );
    }
    else
    {
        TextureManager::Instance()->drawFrame(m_textureID, dest.x, dest.y, m_width, m_height, m_currentRow, m_currentFrame, Game::Instance()->getRenderer()
        );
    }
}

//drawing tiles
void SDLGameObject::draw2(const SDL_Rect& camera)
{
    SDL_Rect dest = { (int)(m_position.getX() - camera.x), (int)(m_position.getY() - camera.y), m_width, m_height };
    
    TextureManager::Instance()->draw(m_textureID, dest.x, dest.y, m_width, m_height, Game::Instance()->getRenderer());

}

//drawing shadow
void SDLGameObject::drawShadow(const SDL_Rect& camera, int shadowSize, int offsetX, int offsetY)
{
    SDL_SetRenderDrawBlendMode(Game::Instance()->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), 0, 0, 0, 128);
    int shadowX = m_position.getX() - camera.x + offsetX;
    int shadowY = m_position.getY() - camera.y + offsetY;
    int shadowRadius = m_width / shadowSize;
    for(int w = 0; w < shadowRadius * 2; w++)
    {
        for(int h = 0; h < shadowRadius * 2; h++)
        {
            int dx = shadowRadius - w;
            int dy = shadowRadius - h;
            if((dx * dx + dy * dy) <= (shadowRadius * shadowRadius))
            {
                SDL_RenderDrawPoint(Game::Instance()->getRenderer(), shadowX + dx, shadowY + dy);
            }
        }
    }
    SDL_SetRenderDrawBlendMode(Game::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
}

void SDLGameObject::drawTitle(const SDL_Rect& camera, const std::string& title, int offsetY) 
{
    if (!Game::Instance()->getFontSmall()) 
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    if (title != m_currentTitle || !m_titleTexture) 
    {
        if (m_titleTexture) 
        {
            SDL_DestroyTexture(m_titleTexture);
            m_titleTexture = nullptr;
        }

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(Game::Instance()->getFontSmall(), title.c_str(), textColor);
        if (!textSurface) 
        {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            return;
        }

        m_titleTexture = SDL_CreateTextureFromSurface(Game::Instance()->getRenderer(), textSurface);
        SDL_FreeSurface(textSurface);
        m_currentTitle = title;
    }

    int textWidth, textHeight;
    SDL_QueryTexture(m_titleTexture, nullptr, nullptr, &textWidth, &textHeight);
    SDL_Rect destRect = {
        static_cast<int>(m_position.getX() - camera.x - textWidth / 2 + m_width / 2),
        static_cast<int>(m_position.getY() - camera.y - offsetY),
        textWidth,
        textHeight
    };
    SDL_RenderCopy(Game::Instance()->getRenderer(), m_titleTexture, nullptr, &destRect);
}

void SDLGameObject::update()
{
    m_position += m_velocity;
}

void SDLGameObject::clean()
{
    if (m_titleTexture) {
        SDL_DestroyTexture(m_titleTexture);
        m_titleTexture = nullptr;
    }
}