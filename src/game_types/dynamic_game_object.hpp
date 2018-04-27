#ifndef dynamic_game_object_hpp
#define dynamic_game_object_hpp

#include <stdio.h>
#include <SDL2/SDL.h>

#include "../physics_engine/dynamic_body.hpp"
#include "../game_types/game_object.hpp"


class DynamicGameObject : public GameObject, public DynamicBody
{
public:
    DynamicGameObject(int x=0, int y=0, int w=64, int h=64, float life_time=0, float friction_coefficient=10, float air_friction_coefficient=10, float mass=1.0, float max_vel_x=10, float max_vel_y=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false, BEHAVIOUR_PRIORITY priority=NOT_PRIMARY);
    virtual void moveTo(int x, int y);
    virtual void init(SDL_Renderer* pt_renderer);

    virtual void drawCollisions();
};


#endif /* dynamic_game_object_hpp */
