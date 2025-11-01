#include "Screen.h"
#include "Vec2D.h"
#include "Utlis.h"
#include "Line2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"
#include <SDL.h>
#include <cassert>
#include <cmath>

Screen::Screen() : mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr)
{
}

Screen::~Screen()
{
    if (moptrWindow)
    {
       SDL_DestroyWindow(moptrWindow);
       moptrWindow = nullptr;  
    }

    SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    mWidth = w;
    mHeight = h;


     moptrWindow = SDL_CreateWindow(
        "Arcade",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        mWidth * mag,
        mHeight * mag,
        0
    );

    if (moptrWindow)
    {
       mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow);
       
       SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;

        Color::InitColorFormat(pixelFormat);

        mClearColor = Color::Black();

        mBackBuffer.Init(pixelFormat->format, mWidth, mHeight);

        mBackBuffer.Clear(mClearColor);

    }


	return moptrWindow;
}

void Screen::SwapScreens()
{
    assert(moptrWindow);

    if (moptrWindow)
    {
        ClearScreen();

        SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

        SDL_UpdateWindowSurface(moptrWindow);

        mBackBuffer.Clear(mClearColor);

    }

    
}

void Screen::Draw(int x, int y, const Color& color)
{
    assert(moptrWindow);

    if (moptrWindow)
    {
        mBackBuffer.SetPixel(color, x, y);// Set Pixel center one
    }
}

void Screen::Draw(const Vec2D& point, const Color& color)
{
    assert(moptrWindow);

    if (moptrWindow)
    {
        mBackBuffer.SetPixel(color, point.GetX(), point.GetY());

    }
}

void Screen::Draw(const Line2D& line, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        int dx, dy;

        int x0 = roundf(line.GetP0().GetX());
        int y0 = roundf(line.GetP0().GetY());
        int x1 = roundf(line.GetP1().GetX());
        int y1 = roundf(line.GetP1().GetY());

        dx = x1 - x0; // width and height of both dx and dy
        dy = y1 - y0;

        signed const char ix((dx > 0) - (dx < 0)); // evaluate to 1 or -1
        signed const char iy((dy > 0) - (dy < 0));

        dx = abs(dx) * 2;
        dy = abs(dy) * 2;

        Draw(x0, y0, color);

        if (dx >= dy)
        {
            // go along in the x

            int d = dy - dx / 2;

            while (x0 != x1)
            {
                if (d >= 0)
                {
                    d -= dx;
                    y0 += iy;
                }

                d += dy;
                x0 += ix;

                Draw(x0, y0, color);
            }

        }
        else
        {
            // go along in the y
            int d = dx - dy / 2;

            while (y0 != y1)
            {
                if (d >= 0)
                {
                    d -= dy;
                    x0 += ix;
                }

                d += dx;
                y0 += iy;

                Draw(x0, y0, color);
            }
        }
    }
}

void Screen::Draw(const Triangle& triangle, const Color& color)
{
    Line2D p0p1 = Line2D(triangle.GetP0(), triangle.GetP1());
    Line2D p1p2 = Line2D(triangle.GetP1(), triangle.GetP2());
    Line2D p2p0 = Line2D(triangle.GetP2(), triangle.GetP0());

    Draw(p0p1, color);
    Draw(p1p2, color);
    Draw(p2p0, color);
}

void Screen::Draw(const AARectangle& rect, const Color& color)
{
    std::vector<Vec2D> points = rect.GetPoints();

    Line2D p0p1 = Line2D(points[0], points[1]);
    Line2D p1p2 = Line2D(points[1], points[2]);
    Line2D p2p3 = Line2D(points[2], points[3]);
    Line2D p3p0 = Line2D(points[3], points[0]);

    Draw(p0p1, color);
    Draw(p1p2, color);
    Draw(p2p3, color);
    Draw(p3p0, color);
}

void Screen::Draw(const Circle& circle, const Color& color)
{
   /* static unsigned int NUM_CIRCLE_SEGMENTS = 30;
    
    float angle = TWO_PI / float(NUM_CIRCLE_SEGMENTS);

    Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
     Vec2D p1 = p0;

    for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
    {
        Line2D nextLineToDraw;
        p1.Rotate(angle, circle.GetCenterPoint());
        std::cout << "seg " << i << ": p0=(" << p0.GetX() << "," << p0.GetY()
            << ") p1=(" << p1.GetX() << "," << p1.GetY() << ")\n";
        nextLineToDraw.SetP1(p1);
        nextLineToDraw.SetP0(p0);
       
        Draw(nextLineToDraw, color);
        p0 = p1;
    }*/

    const unsigned SEG = 64;
    const Vec2D c = circle.GetCenterPoint();
    const float r = circle.GetRadius();
    const float dθ = 2.0f * PI / float(SEG);

    Vec2D p0(c.GetX() + r, c.GetY());
    for (unsigned i = 1; i <= SEG; ++i) {
        float θ = dθ * i;
        Vec2D p1(c.GetX() + r * cosf(θ), c.GetY() + r * sinf(θ));
        Line2D seg; seg.SetP0(p0); seg.SetP1(p1);
        Draw(seg, color);        // must NOT clear/present inside
        p0 = p1;
    }

}

void Screen::ClearScreen()
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
    }
}