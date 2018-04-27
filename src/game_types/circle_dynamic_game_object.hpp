#ifndef circle_dynamic_game_object_hpp
#define circle_dynamic_game_object_hpp

#include <iostream>
#include <SDL2/SDL.h>
//#include <SDL2_image/SDL_image.h>
#include <stdio.h>

#include "dynamic_game_object.hpp"
#include "../physics_engine/circle_dynamic_body.hpp"


using namespace std;

class CircleDynamicGameObject : public DynamicGameObject, public CircleDynamicBody
{
public:
    CircleDynamicGameObject(int x=0, int y=0, int diameter=64, float life_time=0, float friction_coefficient=10, float air_friction_coefficient=10, float mass=1.0, float max_vel_x=10, float max_vel_y=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);


    virtual void init(SDL_Renderer* pt_renderer);
    virtual void drawCollisions(){};

    virtual void update(){GameObject::update();};
    virtual void moveTo(int x, int y){DynamicGameObject::moveTo(x, y);};
};

#endif /* circle_dynamic_game_object_hpp */
