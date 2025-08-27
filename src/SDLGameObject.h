#pragma once
#include "GameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "Vector2D.h"

class SDLGameObject : public GameObject
{
public:
    SDLGameObject(const LoaderParams* pParams);
    virtual void draw(const SDL_Rect& camera) override;
    virtual void draw2(const SDL_Rect& camera);
    virtual void drawShadow(const SDL_Rect& camera, int shadowSize, int offsetX, int offsetY);
    virtual void drawTitle(const SDL_Rect& camera, const std::string& title, int offsetY);
    virtual void update();
    virtual void clean();
    Vector2D& getPosition() { return m_position; }
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    
protected:
    Vector2D m_position;
    Vector2D m_velocity;
    int m_width;
    int m_height;
    int m_currentRow;
    int m_currentFrame;
    std::string m_textureID;

    bool m_visible = true;

    SDL_Texture* m_titleTexture = nullptr;
    std::string m_currentTitle;
};