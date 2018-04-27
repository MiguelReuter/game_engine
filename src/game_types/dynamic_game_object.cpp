#include "dynamic_game_object.hpp"

DynamicGameObject::DynamicGameObject(int x, int y, int w, int h, float life_time, float friction_coefficient, float air_friction_coefficient, float mass, float max_vel_x, float max_vel_y, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom, BEHAVIOUR_PRIORITY priority) : GameObject(x, y, w, h, life_time, ghost_left, ghost_top, ghost_right, ghost_bottom), Body(x, y, w, h,friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom), DynamicBody(x, y, w, h, friction_coefficient, air_friction_coefficient, mass, max_vel_x, max_vel_y, ghost_left, ghost_top, ghost_right, ghost_bottom, priority), Sprite(x, y, w, h)
{}

void DynamicGameObject::init(SDL_Renderer* pt_renderer)
{
    GameObject::init(pt_renderer);
    
    // default sprite texture
    SDL_Surface* pt_surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
    SDL_FillRect(pt_surface, NULL, SDL_MapRGB(pt_surface->format, 100, 100, 100));
    texture = SDL_CreateTextureFromSurface(renderer, pt_surface);
    SDL_FreeSurface(pt_surface);
}

void DynamicGameObject::moveTo(int x, int y)
{
    GameObject::moveTo(x, y);
    DynamicBody::moveTo(x, y);
}

void DynamicGameObject::drawCollisions()
{
    // default sprite texture
    SDL_Surface* pt_surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
    SDL_FillRect(pt_surface, NULL, SDL_MapRGB(pt_surface->format, 100, 100, 100));
    
    int static_marker_size = 8;
    int dynamic_marker_size = 4;
    
    
    // ---------- STATIC COLLISIONS ----------
    if (!static_coll_bodies[LEFT].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, static_marker_size, rect.h, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 200, 0));
        SDL_BlitSurface(coll_surface, NULL, pt_surface, NULL);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!static_coll_bodies[RIGHT].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, static_marker_size, rect.h, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 200, 0));
        SDL_Rect r;
        r.x = rect.w - static_marker_size;
        r.y = 0;
        r.w = static_marker_size;
        r.h = rect.h;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!static_coll_bodies[BOTTOM].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, rect.w, static_marker_size, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 200, 0));
        SDL_Rect r;
        r.x = 0;
        r.y = rect.h - static_marker_size;
        r.w = rect.w;
        r.h = static_marker_size;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!static_coll_bodies[TOP].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, rect.w, static_marker_size, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 200, 0));
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = rect.w;
        r.h = static_marker_size;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    // ---------- DYNAMIC COLLISIONS ----------
    if (!dynamic_coll_bodies[LEFT].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, dynamic_marker_size, rect.h, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 0, 0));
        SDL_BlitSurface(coll_surface, NULL, pt_surface, NULL);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!dynamic_coll_bodies[RIGHT].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, dynamic_marker_size, rect.h, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 0, 0));
        SDL_Rect r;
        r.x = rect.w - dynamic_marker_size;
        r.y = 0;
        r.w = dynamic_marker_size;
        r.h = rect.h;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!dynamic_coll_bodies[BOTTOM].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, rect.w, dynamic_marker_size, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 0, 0));
        SDL_Rect r;
        r.x = 0;
        r.y = rect.h - dynamic_marker_size;
        r.w = rect.w;
        r.h = dynamic_marker_size;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    if (!dynamic_coll_bodies[TOP].empty())
    {
        SDL_Surface* coll_surface = SDL_CreateRGBSurface(0, rect.w, dynamic_marker_size, 32, 0, 0, 0, 0);
        SDL_FillRect(coll_surface, NULL, SDL_MapRGB(pt_surface->format, 200, 0, 0));
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = rect.w;
        r.h = dynamic_marker_size;
        SDL_BlitSurface(coll_surface, NULL, pt_surface, &r);
        SDL_FreeSurface(coll_surface);
    }
    
    
    
    
    SDL_DestroyTexture(texture);
    
    texture = SDL_CreateTextureFromSurface(renderer, pt_surface);
    SDL_FreeSurface(pt_surface);
}

