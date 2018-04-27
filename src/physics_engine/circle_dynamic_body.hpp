#ifndef circle_dynamic_body_hpp
#define circle_dynamic_body_hpp

#include <iostream>
#include "math.h"
#include <stdio.h>

#include "dynamic_body.hpp"

using namespace std;

enum INTERSECT_TYPE {
    LEFT_INTERSECT = 0,
    TOP_INTERSECT = 1,
    RIGHT_INTERSECT = 2,
    BOTTOM_INTERSECT = 3,
    BOTTOM_LEFT_INTERSECT = 4,
    TOP_LEFT_INTERSECT = 5,
    TOP_RIGHT_INTERSECT = 6,
    BOTTOM_RIGHT_INTERSECT = 7
};



class CircleDynamicBody : public DynamicBody
{
public:
    CircleDynamicBody(int x=0, int y=0, int diameter=64, float friction_coefficient=10, float air_friction_coefficient=10, float mass=1.0, float max_vel_x=10, float max_vel_y=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);
    
    void update(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
    void resolveCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
    
protected:
    //void updateCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
    bool intersectCircleRect(const SDL_Rect* A_circle, const SDL_Rect* B, SDL_Point* result, int* intersect_type);
    
    void applyNewtonLaw();
    void updateCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
};


#endif /* circle_dynamic_body_hpp */
