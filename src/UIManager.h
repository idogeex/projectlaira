#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "TextureManager.h"

class UIManager
{
public: 
    UIManager(SDL_Renderer* renderer);
    void renderPlayerStatus(const Player& player, const SDL_Rect& camera);
    void renderEnemyWindow(Enemy* target, const SDL_Rect& camera);
    bool handleMouseClick(int x, int y);

    void renderTutorialWindow(const SDL_Rect& camera);
    void setTutorialActive(bool active) { m_showTutorial = active; }
    void setTutorialText(const std::vector<std::string>& text) { m_tutorialText = text; }
    bool handleTutorialInput();
    bool isTutorialActive() { return m_showTutorial; }

private:
    SDL_Renderer* m_renderer;
    SDL_Rect m_closeBtn = {0, 0, 0, 0};

    bool m_showTutorial = false;
    std::vector<std::string> m_tutorialText;

    void renderBar(int x, int y, int w, int h, int current, int max, SDL_Color color, const std::string& label);
};