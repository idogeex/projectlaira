#pragma once
#include <iostream>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:

    static TextureManager* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new TextureManager();
            return s_pInstance;
        }

        return s_pInstance;
    }

    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    void draw(std::string id, int x, int y, int widht, int height, SDL_Renderer* pRenderer, SDL_RendererFlip = SDL_FLIP_NONE);
    void drawTile(std::string textureID, int srcX, int srcY, int x, int y, int width, int height, SDL_Renderer* renderer); 
    void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip = SDL_FLIP_NONE);
    void clearFromTextureMap(std::string id) { m_textureMap.erase(id); }
    SDL_Texture* getTexture(const std::string& id);
private:
    TextureManager() {};
    static TextureManager* s_pInstance;
    std::map<std::string, SDL_Texture*> m_textureMap;
};