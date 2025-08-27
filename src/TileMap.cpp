#include "TileMap.h"

// load method for map csv
bool TileMap::loadFromCSV(const std::string& fileName)
{
    std::ifstream file(fileName);
    if(!file.is_open())
    {
        std::cerr << "Failed to open: " << fileName << std::endl;
        return false;
    }

    m_mapData.clear();
    std::string line;
    while (std::getline(file, line))
    {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ','))
        {
            if(!cell.empty())
            {
                row.push_back(std::stoi(cell));
            }
        }
        if(!row.empty())
        {
            m_mapData.push_back(row);
        }
    }
    m_rows = m_mapData.size();
    m_cols = m_rows > 0 ? m_mapData[0].size() : 0;
    return true;
}

// load method for collision map
bool TileMap::loadFromCSV2(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open: " << fileName << std::endl;
        return false;
    }

    m_collisionData.clear();
    std::string line;
    while (std::getline(file, line))
    {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ','))
        {
            if (!cell.empty())
            {
                row.push_back(std::stoi(cell));
            }
        }
        if (!row.empty())
        {
            m_collisionData.push_back(row);
        }
    }
    m_rows = m_collisionData.size();
    m_cols = m_rows > 0 ? m_collisionData[0].size() : 0;
    return true;
}

void TileMap::draw(SDL_Renderer* renderer, const SDL_Rect& camera, const std::string& tilesetID, int tileWidth, int tileHeight)
{
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    SDL_Texture* tilesetTexture = TextureManager::Instance()->getTexture(tilesetID);
    if (!tilesetTexture) 
    {
        std::cout << "Tileset texture is not loaded!\n";
        return;
    }
    
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            int tileID = m_mapData[row][col];
            if (tileID < 0) 
            {
                continue;
            }

            int srcX, srcY;
            if (tileID < 15) 
            {
                srcX = tileID * tileWidth;
                srcY = 0;
            } 
            else if (tileID < 30) 
            {
                srcX = (tileID - 15) * tileWidth;
                srcY = tileHeight;
            } 
            else if (tileID < 45)
            {
                srcX = (tileID - 30) * tileWidth;
                srcY = 2 * tileHeight;
            }
            else
            {
                srcX = (tileID - 45) * tileWidth;
                srcY = 3 * tileHeight;
            }

            int worldX = col * tileWidth;
            int worldY = row * tileHeight;

            SDL_Rect destRect = { worldX - camera.x, worldY - camera.y, tileWidth, tileHeight};
            SDL_Rect srcRect = { srcX, srcY, tileWidth, tileHeight};
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
        }
    }
}

void TileMap::draw2(SDL_Renderer* renderer, const SDL_Rect& camera, const std::string& tilesetID, int tileWidth, int tileHeight)
{
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    SDL_Texture* tilesetTexture = TextureManager::Instance()->getTexture(tilesetID);
    if (!tilesetTexture) 
    {
        std::cout << "Tileset texture is not loaded!\n";
        return;
    }
    
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            int tileID = m_mapData[row][col];
            if (tileID < 0) 
            {
                continue;
            }

            int srcX, srcY;
            if (tileID < 5) 
            {
                srcX = tileID * tileWidth;
                srcY = 0;
            } 
            else if (tileID < 10) 
            {
                srcX = (tileID - 5) * tileWidth;
                srcY = tileHeight;
            } 
            else
            {
                srcX = (tileID - 10) * tileWidth;
                srcY = 2 * tileHeight;
            }

            int worldX = col * tileWidth;
            int worldY = row * tileHeight;

            SDL_Rect destRect = { worldX - camera.x, worldY - camera.y, tileWidth, tileHeight};
            SDL_Rect srcRect = { srcX, srcY, tileWidth, tileHeight};
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
        }
    }
}

bool TileMap::isCollision(int x, int y) const
{
    if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    {
        return false; 
    }
    return m_collisionData[y][x] == 0; 
}