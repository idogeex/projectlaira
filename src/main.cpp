#include "Game.h"   

const int FPS = 60;
const int frameDelay = 1000.0f / FPS;

int main(int argc, char* args[])
{
    Uint32 frameStart, frameTime;

    std::cout << "game init attempt...\n";

    if(Game::Instance()->init("Laira", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false))
    {   
        std::cout << "game init success\n";
        while(Game::Instance()->running())
        {
            frameStart = SDL_GetTicks();
            Game::Instance()->handleEvents();
            Game::Instance()->update();
            Game::Instance()->render();

            frameTime = SDL_GetTicks() - frameStart;
            if(frameTime < frameDelay)
            {
                SDL_Delay((int)(frameDelay - frameTime));
            }
        }
    }
    else
    {
        std::cout << "game init failure - " << SDL_GetError() << "\n";
        return -1;
    }
    std::cout << "game closing...\n";
    Game::Instance()->clean();
    
    return 0;
}