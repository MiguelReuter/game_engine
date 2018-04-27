#ifndef sprite_hpp
#define sprite_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>


using namespace std;

class Sprite
{
public:
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;
    
    
    virtual void init(SDL_Renderer* pt_renderer);
    virtual void update();
    virtual void moveTo(int x, int y) { rect.x = x; rect.y = y;};
    
    virtual int getXCenter(){return rect.x + rect.w / 2;};
    virtual int getYCenter(){return rect.y + rect.h / 2;};
    
    Sprite(int x, int y, int w, int h);
    virtual ~Sprite();
};

#endif /* sprite_hpp */
