#pragma once
#include <iostream> 
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <SDL2/SDL.h>

#include "TextureManager.h"

class TileMap
{
public:
    bool loadFromCSV(const std::string& fileName); // loading MAP FILE
    bool loadFromCSV2(const std::string& fileName); // loading COLLISION FILE
    void draw(SDL_Renderer* renderer, const SDL_Rect& camera, const std::string& tilesetID, int tileWidth, int tileHeight);
    void draw2(SDL_Renderer* renderer, const SDL_Rect& camera, const std::string& tilesetID, int tileWidth, int tileHeight);
    bool isCollision(int x, int y) const;
    
    int getWidth() const { return m_cols * m_tileWidth; }
    int getHeight() const { return m_rows * m_tileHeight; }

    int getWidthInTiles() const { return m_cols; }
    int getHeightInTiles() const { return m_rows; }
private:
    std::vector<std::vector<int>> m_mapData;
    std::vector<std::vector<int>> m_collisionData;
    int m_rows = 0;
    int m_cols = 0;
    int m_tileWidth = 32;
    int m_tileHeight = 32;
};