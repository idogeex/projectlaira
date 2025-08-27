#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* renderer) : m_renderer(renderer)
{

}

void UIManager::renderBar(int x, int y, int w, int h, int current, int max, SDL_Color color, const std::string& label)
{
    // status bars background
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    SDL_Rect bgRect = {x, y, w, h};
    SDL_RenderFillRect(m_renderer, &bgRect);

    // colors
    float ratio = static_cast<float>(current)/max;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect fillRect = {x+2, y+2, static_cast<int>((w-4)*ratio), h-4};
    SDL_RenderFillRect(m_renderer, &fillRect);

    // text
    std::string text;
    if(label == "EXP") 
    {
        text = label + ": " + std::to_string(current) + "%";
    } 
    else if(label == " ")
    {
        text = label + " ";
    }
    else 
    {
        text = label + ": " + std::to_string(current) + " /" + std::to_string(max);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(Game::Instance()->getFontSmall(), text.c_str(), {255, 255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_Rect textDest = {x + 5, y + (h - surface->h)/2, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, NULL, &textDest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void UIManager::renderPlayerStatus(const Player& player, const SDL_Rect& camera) 
{
    //status bars position is const because it is always in the left upper corner and will not be moved if camera dimensions will change
    const int startX = 10;
    int startY = 10;
    const int barWidth = 100;
    const int barHeight = 12;
    const int spacing = 1;

    // HP
    renderBar(startX, startY, barWidth, barHeight, player.getHP(), player.getMaxHP(), {255,0,0,255}, "HP");
    // MP
    startY += barHeight + spacing;
    renderBar(startX, startY, barWidth, barHeight, player.getMP(), player.getMaxMP(), {0,0,255,255}, "MP");
    // EXP
    startY += barHeight + spacing;
    renderBar(startX, startY, barWidth, barHeight, (int)(player.getEXPPercent()), 100, {192,192,192,255}, "EXP");
}

void UIManager::renderEnemyWindow(Enemy* target, const SDL_Rect& camera) 
{
    if(!target || !target->isAlive())
    {
        m_closeBtn = {0, 0, 0, 0};
        return;
    }
    SDL_Rect windowRect = { (camera.w - 100) / 2, 20, 100, 40};

    // background
    SDL_SetRenderDrawColor(m_renderer, 40, 40, 40, 220);
    SDL_RenderFillRect(m_renderer, &windowRect);
    // border
    SDL_SetRenderDrawColor(m_renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(m_renderer, &windowRect);
    // HP bar
    renderBar(windowRect.x + 5, windowRect.y + 20, 90, 15, target->getHealth(), target->getMaxHealth(), {200, 30, 30, 255}, " ");
    const std::string& enemyName = target->getName();
    if (enemyName.empty()) 
    {
        std::cerr << "Enemy name is empty!\n";
        return;
    }

    // Target name
    SDL_Surface* surface = TTF_RenderText_Blended(Game::Instance()->getFontSmall(), enemyName.c_str(), {255, 255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_Rect textRect = {windowRect.x + 5, windowRect.y + 5, surface->w, surface->h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// UI clicks 
bool UIManager::handleMouseClick(int x, int y) 
{
    if (x >= m_closeBtn.x && y >= m_closeBtn.y && x <= m_closeBtn.x + m_closeBtn.w && y <= m_closeBtn.y + m_closeBtn.h) 
    {
        return true; 
    }
    return false;
}

void UIManager::renderTutorialWindow(const SDL_Rect& camera) 
{
    if(!m_showTutorial) return;

    SDL_Renderer* renderer = Game::Instance()->getRenderer();

    SDL_Rect windowRect = {camera.w/2 - 200, camera.h/2 - 150, 400, 300 };
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 220);
    SDL_RenderFillRect(renderer, &windowRect);

    int yPos = windowRect.y + 20;
    for(const auto& line : m_tutorialText) 
    {
        SDL_Surface* surface = TTF_RenderText_Blended(Game::Instance()->getFontSmall(), line.c_str(), {255, 255, 255, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {windowRect.x + 20, yPos, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        yPos += surface->h + 15;
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_Surface* closeHint = TTF_RenderText_Blended(Game::Instance()->getFontSmall(), "Press space to continue...", {200, 200, 200, 255});
    SDL_Rect hintDest = { windowRect.x + (windowRect.w - closeHint->w)/2, windowRect.y + windowRect.h - 40, closeHint->w,closeHint->h };
    SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, closeHint);
    SDL_RenderCopy(renderer, hintTexture, nullptr, &hintDest);
    
    SDL_FreeSurface(closeHint);
    SDL_DestroyTexture(hintTexture);
}

bool UIManager::handleTutorialInput() 
{
    if(m_showTutorial && InputHandler::Instance()->isKeyPressed(SDL_SCANCODE_SPACE)) 
    {
        m_showTutorial = false;
        return true; 
    }
    return false;
}