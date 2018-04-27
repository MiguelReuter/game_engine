#include "sprite.hpp"


using namespace std;

Sprite::Sprite(int x, int y, int w, int h)
{
    angle = 0.0;
    center.x = w / 2;
    center.y = h / 2;
    flip = SDL_FLIP_NONE;
    
    // rect initialization
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void Sprite::init(SDL_Renderer *pt_renderer)
{
    renderer = pt_renderer;
}

void Sprite::update(){
    //cout<<"sprite update"<<endl;
}

Sprite::~Sprite()
{
    //SDL_DestroyTexture(texture);
}
