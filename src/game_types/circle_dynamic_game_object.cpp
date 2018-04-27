#include "circle_dynamic_game_object.hpp"

CircleDynamicGameObject::CircleDynamicGameObject(int x, int y, int diameter, float life_time, float friction_coefficient, float air_friction_coefficient, float mass, float max_vel_x, float max_vel_y, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom) : Body(x, y, diameter, diameter, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom), CircleDynamicBody(x, y, diameter, friction_coefficient, air_friction_coefficient, mass, max_vel_x, max_vel_y, ghost_left, ghost_top, ghost_right, ghost_bottom), DynamicGameObject(x, y, diameter, diameter, life_time, friction_coefficient, air_friction_coefficient, mass, max_vel_x, max_vel_y, ghost_left, ghost_top, ghost_right, ghost_bottom), Sprite(x, y, diameter, diameter)
{}

void CircleDynamicGameObject::init(SDL_Renderer *pt_renderer)
{
    DynamicGameObject::init(pt_renderer);
    /*
    SDL_DestroyTexture(texture);
    
    // default sprite texture
    SDL_Surface* circle = IMG_Load("/Users/lol/Developer/SDL_test/SDL_test/circle.png");
    texture = SDL_CreateTextureFromSurface(renderer, circle);
    
    SDL_FreeSurface(circle);*/
}

