#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <iostream>
#include "Utils/Vec2D.h"
#include "Graphics/Color.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;

using namespace std;

void SetPixel(SDL_Surface* noptrWindowSurface, uint32_t color, int x, int y);
size_t GetIndex(SDL_Surface* noptrSurface, int r, int c);


int main(int argc, const char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* optrWindow = SDL_CreateWindow(
        "Arcade",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if (!optrWindow)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Surface* noptrWindowSurface = SDL_GetWindowSurface(optrWindow);
    
    SDL_PixelFormat* pixelFormat = noptrWindowSurface->format;

    Color::InitColorFormat(pixelFormat);

    

    SetPixel(noptrWindowSurface,Color::Blue().GetPixelColor(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    SDL_UpdateWindowSurface(optrWindow);

    SDL_Event sdlEvent;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }
    }

    //SDL_Delay(2000); // Keep window open for 2 seconds

    SDL_DestroyWindow(optrWindow);
    SDL_Quit();
    return 0;
}

void SetPixel(SDL_Surface* noptrWindowSurface, uint32_t color, int x, int y)
{
    SDL_LockSurface(noptrWindowSurface);

    uint32_t* pixel = (uint32_t*)noptrWindowSurface->pixels;

    size_t index = GetIndex(noptrWindowSurface, y, x);

    pixel[index] = color;

    SDL_UnlockSurface(noptrWindowSurface);
}

size_t GetIndex(SDL_Surface* noptrSurface, int r, int c)
{
    return r * noptrSurface->w + c;
}
