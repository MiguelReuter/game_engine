#ifndef game_object_hpp
#define game_object_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

#include "../physics_engine/body.hpp"
#include "../events_engine/user_events.h"
#include "../graphics_engine/sprite.hpp"


using namespace std;

class GameObject : public virtual Sprite, public virtual Body
{
public:
    GameObject(int x=0, int y=0, int w=64, int h=64, float life_time=0, float friction_coefficient=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);
    virtual void moveTo(int x, int y);

    virtual void update();
    virtual void init(SDL_Renderer* pt_renderer);

    virtual void addObject(GameObject* object, int x=0, int y=0, float dx=0, float dy=0);



    float life_time; // life time in s; 0 if permanent
    int creation_tick;

    virtual ~GameObject(){};
};






#endif /* game_object_hpp */
