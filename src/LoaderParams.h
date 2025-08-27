#pragma once
#include <iostream>

class LoaderParams
{
public:
    LoaderParams(int x, int y, int width, int height, int currentFrame, int currentRow, std::string textureID) : m_x(x), m_y(y), m_width(width), m_height(height), m_currentFrame(currentFrame), m_currentRow(currentRow), m_textureID(textureID), m_zIndex(0)
    {

    }

    LoaderParams(int x, int y, int width, int height, int currentFrame, int currentRow, std::string textureID, int zIndex) : m_x(x), m_y(y), m_width(width), m_height(height), m_currentFrame(currentFrame), m_currentRow(currentRow), m_textureID(textureID), m_zIndex(zIndex)
    {

    }

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getCurrentFrame() const { return m_currentFrame; }
    int getCurrentRow() const { return m_currentRow; }
    std::string getTextureID() const { return m_textureID; }
    
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_currentFrame;
    int m_currentRow;
    std::string m_textureID;
    int m_zIndex;
};