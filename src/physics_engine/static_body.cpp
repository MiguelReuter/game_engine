#include "static_body.hpp"
#include <iostream>

using namespace std;

StaticBody::StaticBody(int x, int y, int w, int h, float friction_coefficient, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom) : Body(x, y, w, h)
{
    this->friction_coefficient = friction_coefficient;
    ghost_sides[LEFT] = ghost_left;
    ghost_sides[TOP] = ghost_top;
    ghost_sides[RIGHT] = ghost_right;
    ghost_sides[BOTTOM] = ghost_bottom;
}

void StaticBody::update()
{
    Body::update();
}

void StaticBody::move(float dx, float dy)
{
    Body::move(dx, dy);
    
    dxy[0] = round(dx);
    dxy[1] = round(dy);
}

