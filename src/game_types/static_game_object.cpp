#include "static_game_object.hpp"

StaticGameObject::StaticGameObject(int x, int y, int w, int h, float life_time, float friction_coefficient, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom) : GameObject(x, y, w, h, life_time, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom), StaticBody(x, y, w, h, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom), Body(x, y, w, h, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom), Sprite(x, y, w, h)

{}

void StaticGameObject::init(SDL_Renderer* pt_renderer)
{
    GameObject::init(pt_renderer);
    
    // default sprite texture
    SDL_Surface* pt_surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
    SDL_FillRect(pt_surface, NULL, SDL_MapRGB(pt_surface->format, 150, 150, 150));
    texture = SDL_CreateTextureFromSurface(renderer, pt_surface);
    SDL_FreeSurface(pt_surface);
}

void StaticGameObject::update()
{
    GameObject::update();
    //StaticBody::update();
}

void StaticGameObject::moveTo(int x, int y)
{
    GameObject::moveTo(x, y);
    StaticBody::moveTo(x, y);
}

void StaticGameObject::move(float dx, float dy)
{
    StaticBody::move(dx, dy);
    rect = bounding_rect;
}

