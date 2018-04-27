#ifndef StaticBody_hpp
#define StaticBody_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include "body.hpp"

class StaticBody : public virtual Body
{
public:
    StaticBody(int x=0, int y=0, int w=64, int h=64,
               float friction_coefficient = 10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);
    
    virtual void update();
    virtual void move(float dx, float dy);
};

#endif /* StaticBody_hpp */
