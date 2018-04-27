#ifndef physics_body_hpp
#define physics_body_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

#include "../constants.h"
#include "static_body.hpp"
#include "body.hpp"


using namespace std;

enum BEHAVIOUR_PRIORITY {
    PRIMARY,
    NOT_PRIMARY
};

class DynamicBody : public virtual Body
{

public:
    // physics attributes
    float air_friction_coefficient;

    // physics variable
    float current_max_dxy[2];
    float absolute_max_dxy[2];
    bool is_falling;
    float force[2];
    float collision_force[2];
    float mass;

    // methods
    virtual void addForce(float fx, float fy){force[0] += fx; force[1] += fy;};
    virtual void addCollisionForce(float fx, float fy){collision_force[0] += fx; collision_force[1] += fy;};

    virtual float getAbsolutePlatformDx();
    virtual float getAbsolutePlatformDy();

    virtual bool canMoveAtLeft(){return (static_coll_bodies[LEFT].empty() and dynamic_coll_bodies[LEFT].empty());};
    virtual bool canMoveAtRight(){return (static_coll_bodies[RIGHT].empty() and dynamic_coll_bodies[RIGHT].empty());};
    virtual bool canMoveAtTop(){return (static_coll_bodies[TOP].empty() and dynamic_coll_bodies[TOP].empty());};

    virtual bool isPrimaryDynamic(){return dynamics_priority==PRIMARY;};

    float getEquivalentMass();
    virtual vector<DynamicBody*> getTopDynamicBodies();
    virtual void update(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
    virtual void setMove(float dx, float dy);

    vector<Body*> getGhostCollBodies(){return ghost_coll_bodies;};


    DynamicBody(int x=0, int y=0, int w=64, int h=64, float friction_coefficient=10, float air_friction_coefficient=10, float mass=1.0, float max_vel_x=10, float max_vel_y=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false, BEHAVIOUR_PRIORITY priority=NOT_PRIMARY, bool ghost_for_primary=false);

protected:


    // collisions attributes
    vector<StaticBody*> static_coll_bodies[4];
    vector<DynamicBody*> dynamic_coll_bodies[4];
    vector<Body*> ghost_coll_bodies;

    virtual void applyNewtonLaw();
    virtual void updateCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);
    virtual void resolveCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies);

    BEHAVIOUR_PRIORITY dynamics_priority;
};
#endif /* dynamic_body_hpp */
