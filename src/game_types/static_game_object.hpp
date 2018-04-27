#ifndef static_game_object_hpp
#define static_game_object_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

#include "../physics_engine/static_body.hpp"
#include "game_object.hpp"


class StaticGameObject : public GameObject, public StaticBody
{
public:
    StaticGameObject(int x=0, int y=0, int w=64, int h=64, float life_time=0, float friction_coefficient=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);
    virtual void moveTo(int x, int y);
    virtual void move(float dx, float dy);

    virtual void update();
    virtual void init(SDL_Renderer* pt_renderer);
};

#endif /* static_game_object_hpp */
