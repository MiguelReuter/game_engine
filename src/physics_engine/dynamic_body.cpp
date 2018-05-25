#include "dynamic_body.hpp"
#include <math.h>
#include <algorithm>

DynamicBody::DynamicBody(int x, int y, int w, int h, float friction_coefficient, float air_friction_coefficient, float mass, float max_dx, float max_dy, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom, BEHAVIOUR_PRIORITY priority, bool ghost_for_primary) : Body(x, y, w, h, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom)
{
    // Physics variable
    force[0] = .0; force[1] = .0;
    collision_force[0] = .0; collision_force[1] = .0;

    this->air_friction_coefficient = air_friction_coefficient;
    this->mass = mass;

    absolute_max_dxy[0] = NOMINAL_DT * PIXELS_NB_PER_METER * max_dx;
    absolute_max_dxy[1] = NOMINAL_DT * PIXELS_NB_PER_METER * max_dy;

    current_max_dxy[0] = absolute_max_dxy[0];
    current_max_dxy[1] = absolute_max_dxy[1];

    is_falling = true;
    on_body = nullptr;

    dynamics_priority = priority;
    ghost_for_primary_dynamic_object = ghost_for_primary;
}

void DynamicBody::setMove(float dx, float dy)
{
    force[0] = 0; force[1] = 0;
    dxy[0] = dx; dxy[1] = dy;
}


void DynamicBody::update(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies)
{
    previous_bounding_rect = bounding_rect;
    applyModificationsOnBoundingRect();
    applyNewtonLaw();
    resolveCollisions(static_bodies, dynamic_bodies);
}


void DynamicBody::updateCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies)
{
    SDL_Rect coll_rect, overlap_rect;
    coll_rect.x = bounding_rect.x - 1;
    coll_rect.y = bounding_rect.y - 1;
    coll_rect.w = bounding_rect.w + 2;
    coll_rect.h = bounding_rect.h + 2;

    // reset
    for (unsigned int i=0; i<4; i++) {
        static_coll_bodies[i].clear();
        dynamic_coll_bodies[i].clear();
    }
    ghost_coll_bodies.clear();

    // ---------- STATIC COLLISIONS ----------
    for (unsigned int i=0; i<static_bodies.size(); i++) {
        StaticBody* object = static_bodies[i];
        SDL_Rect* obj_rect = &object->bounding_rect;

        if (object->bounding_rect.x == 96 and  object->bounding_rect.y  == 160)
            ;

        if (SDL_IntersectRect(&coll_rect, obj_rect, &overlap_rect) == SDL_TRUE) {
            // ghost object
            if(object->ghost_sides[LEFT] or object->ghost_sides[TOP] or object->ghost_sides[RIGHT] or object->ghost_sides[BOTTOM])
                ghost_coll_bodies.push_back(object);

            // bottom collision
            if ( !ghost_sides[BOTTOM] and !object->ghost_sides[TOP] ) {
                if ( overlap_rect.y + overlap_rect.h <= coll_rect.y + coll_rect.h and (overlap_rect.x + overlap_rect.w > coll_rect.x + 1 and overlap_rect.x < coll_rect.x + coll_rect.w - 1) ) {
                    if ((object->bounding_rect.y <= bounding_rect.y + bounding_rect.h) and (object->previous_bounding_rect.y >= previous_bounding_rect.y + previous_bounding_rect.h))
                        static_coll_bodies[BOTTOM].push_back(object);
                }
            }

            // top collision
            if ( !ghost_sides[TOP] and !object->ghost_sides[BOTTOM] ) {
                if ( overlap_rect.y >= coll_rect.y and (overlap_rect.x + overlap_rect.w > coll_rect.x + 1 and overlap_rect.x < coll_rect.x + coll_rect.w - 1) ) {
                    if ((bounding_rect.y <= object->bounding_rect.y + object->bounding_rect.h) and (previous_bounding_rect.y >= object->previous_bounding_rect.y + object->previous_bounding_rect.h))
                        static_coll_bodies[TOP].push_back(object);
                }
            }

            // right collision
            if ( !ghost_sides[RIGHT] and !object->ghost_sides[LEFT] ) {
                if ( overlap_rect.x + overlap_rect.w == coll_rect.x + coll_rect.w and overlap_rect.y < coll_rect.y + coll_rect.h - 1 and overlap_rect.y + overlap_rect.h > coll_rect.y + 1) {
                    if ((object->bounding_rect.x <= bounding_rect.x + bounding_rect.w) and (object->previous_bounding_rect.x >= previous_bounding_rect.x + previous_bounding_rect.w))
                        static_coll_bodies[RIGHT].push_back(object);
                }
            }

            // left collision
            if ( !ghost_sides[LEFT] and !object->ghost_sides[RIGHT] ) {
                if ( overlap_rect.x == coll_rect.x and overlap_rect.y < coll_rect.y + coll_rect.h - 1 and overlap_rect.y + overlap_rect.h > coll_rect.y + 1) {
                    if ((bounding_rect.x <= object->bounding_rect.x + object->bounding_rect.w) and (previous_bounding_rect.x >= object->previous_bounding_rect.x + object->previous_bounding_rect.w))
                        static_coll_bodies[LEFT].push_back(object);
                }
            }
        }
    }
    // ---------- DYNAMIC COLLISIONS ----------
    for (unsigned int i=0; i<dynamic_bodies.size(); i++) {
        DynamicBody* object = dynamic_bodies[i];
        if (this != object) {
            SDL_Rect* obj_rect = &object->bounding_rect;
            if (SDL_IntersectRect(&coll_rect, obj_rect, &overlap_rect) == SDL_TRUE) {
                // ghost object
                if(object->ghost_sides[LEFT] and object->ghost_sides[TOP] and object->ghost_sides[RIGHT] and object->ghost_sides[BOTTOM])
                    ghost_coll_bodies.push_back(object);

                // bottom collision
                if ( !ghost_sides[BOTTOM] and !object->ghost_sides[TOP] ) {
                    if ( overlap_rect.y + overlap_rect.h <= coll_rect.y + coll_rect.h and ( overlap_rect.x + overlap_rect.w > coll_rect.x + 1 and overlap_rect.x < coll_rect.x + coll_rect.w - 1) ) {
                        if ((object->bounding_rect.y <= bounding_rect.y + bounding_rect.h) and (object->previous_bounding_rect.y >= previous_bounding_rect.y + previous_bounding_rect.h))
                            dynamic_coll_bodies[BOTTOM].push_back(object);
                    }
                }

                // top collision
                if ( !ghost_sides[TOP] and !object->ghost_sides[BOTTOM] ) {
                    if ( overlap_rect.y >= coll_rect.y and overlap_rect.x + overlap_rect.w > coll_rect.x + 1 and overlap_rect.x < coll_rect.x + coll_rect.w - 1 ) {
                        if ((bounding_rect.y <= object->bounding_rect.y + object->bounding_rect.h) and (previous_bounding_rect.y >= object->previous_bounding_rect.y + object->previous_bounding_rect.h))
                            dynamic_coll_bodies[TOP].push_back(object);
                    }
                }
                // right collision
                if ( !ghost_sides[RIGHT] and !object->ghost_sides[LEFT] ) {
                    if ( (overlap_rect.x + overlap_rect.w) <= (coll_rect.x + coll_rect.w) and overlap_rect.y < coll_rect.y + coll_rect.h - 1 and overlap_rect.y + overlap_rect.h > coll_rect.y + 1) {
                        if ((object->bounding_rect.x <= bounding_rect.x + bounding_rect.w) and (object->previous_bounding_rect.x >= previous_bounding_rect.x + previous_bounding_rect.w))
                            dynamic_coll_bodies[RIGHT].push_back(object);
                    }
                }

                // left collision
                if ( !ghost_sides[LEFT] and !object->ghost_sides[RIGHT] ) {
                    if ( overlap_rect.x >= coll_rect.x and overlap_rect.y < coll_rect.y + coll_rect.h - 1 and overlap_rect.y + overlap_rect.h > coll_rect.y + 1 ) {
                        if ((bounding_rect.x <= object->bounding_rect.x + object->bounding_rect.w) and (previous_bounding_rect.x >= object->previous_bounding_rect.x + object->previous_bounding_rect.w))
                            dynamic_coll_bodies[LEFT].push_back(object);
                    }
                }
            }
        }
    }

    // if primary priority and objects are ghost for primary object
    if (dynamics_priority == PRIMARY) {
        for (unsigned int i=0; i<4; i++) {
            for (unsigned int j=0; j<dynamic_coll_bodies[i].size(); j++) {
                if (dynamic_coll_bodies[i][j]->ghost_for_primary_dynamic_object) {
                    ghost_coll_bodies.push_back(dynamic_coll_bodies[i][j]);
                    dynamic_coll_bodies[i].erase(dynamic_coll_bodies[i].begin() + j);
                    j -= 1;
                }
            }
        }
    }
}


void DynamicBody::resolveCollisions(vector<StaticBody *> static_bodies, vector<DynamicBody*> dynamic_bodies)
{
    updateCollisions(static_bodies, dynamic_bodies);
    is_falling = true;

    vector<Body*> bodies; // a supprimer
    // ---------- LEFT COLLISIONS ----------
    // static
    for (unsigned int i=0; i<static_coll_bodies[LEFT].size(); i++) {
        StaticBody* object = static_coll_bodies[LEFT][i];
        if (canMoveAtRight() and bounding_rect.x <= object->bounding_rect.x + object->bounding_rect.w) {
            dxy[0] = max(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
            bounding_rect.x = object->bounding_rect.x + object->bounding_rect.w;
        }
    }
    // dynamic
    for (unsigned int i=0; i<dynamic_coll_bodies[LEFT].size(); i++) {
        DynamicBody* object = dynamic_coll_bodies[LEFT][i];
        if (dynamics_priority == PRIMARY) {
            if (bounding_rect.x <= object->bounding_rect.x + object->bounding_rect.w) {
                dxy[0] = max(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
                bounding_rect.x = object->bounding_rect.x + object->bounding_rect.w;
                if (object->canMoveAtLeft()) {
                    float coll_force = min(float(force[0] * mass / (mass + object->getEquivalentMass())), .0f);
                    object->addCollisionForce(coll_force, 0);
                }
            }
        }
        else {
            if (canMoveAtRight() and bounding_rect.x <= object->bounding_rect.x + object->bounding_rect.w) {
                dxy[0] = max(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
                bounding_rect.x = object->bounding_rect.x + object->bounding_rect.w;
            }
        }
    }
    updateCollisions(static_bodies, dynamic_bodies);

    // ---------- RIGHT COLLISIONS ----------
    // static
    for (unsigned int i=0; i<static_coll_bodies[RIGHT].size(); i++) {
        StaticBody* object = static_coll_bodies[RIGHT][i];
        if (canMoveAtLeft() and bounding_rect.x >= object->bounding_rect.x - bounding_rect.w) {
            dxy[0] = min(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
            bounding_rect.x = object->bounding_rect.x - bounding_rect.w;
        }
    }
    // dynamic
    for (unsigned int i=0; i<dynamic_coll_bodies[RIGHT].size(); i++) {
        DynamicBody* object = dynamic_coll_bodies[RIGHT][i];
        if (dynamics_priority == PRIMARY) {
            if (bounding_rect.x >= object->bounding_rect.x - bounding_rect.w) {
                dxy[0] = min(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
                bounding_rect.x = object->bounding_rect.x - bounding_rect.w;

                if (object->canMoveAtRight()) {
                    float coll_force = max(float(force[0] * mass / (mass + object->getEquivalentMass())), .0f);
                    object->addCollisionForce(coll_force, 0);
                }
            }
        }
        else {
            if (canMoveAtLeft() and bounding_rect.x >= object->bounding_rect.x - bounding_rect.w) {
                dxy[0] = min(object->dxy[0] - getAbsolutePlatformDx(), dxy[0]);
                bounding_rect.x = object->bounding_rect.x - bounding_rect.w;
            }
        }
    }
    updateCollisions(static_bodies, dynamic_bodies);

    // ---------- TOP COLLISIONS ----------
    // static
    for (unsigned int i=0; i<static_coll_bodies[TOP].size(); i++) {
        StaticBody* object = static_coll_bodies[TOP][i];
        if (bounding_rect.y <= object->bounding_rect.y + object->bounding_rect.h) {
            //dxy[1] -= bounding_rect.y - (object->bounding_rect.y + object->bounding_rect.h);
            dxy[1] = max(dxy[1], object->dxy[1]);
            bounding_rect.y = object->bounding_rect.y + object->bounding_rect.h;
        }
    }
    // dynamic
    for (unsigned int i=0; i<dynamic_coll_bodies[TOP].size(); i++) {
        DynamicBody* object = dynamic_coll_bodies[TOP][i];
        if (bounding_rect.y <= object->bounding_rect.y + object->bounding_rect.h) {
            //dxy[1] -= bounding_rect.y - (object->bounding_rect.y + object->bounding_rect.h);
            dxy[1] = max(dxy[1], object->dxy[1]);
            bounding_rect.y = object->bounding_rect.y + object->bounding_rect.h;
        }
    }
    updateCollisions(static_bodies, dynamic_bodies);

    // ---------- BOTTOM COLLISIONS ----------
    int max_px_on_platform = 0;
    // static
    for (unsigned int i=0; i<static_coll_bodies[BOTTOM].size(); i++) {
        StaticBody* object = static_coll_bodies[BOTTOM][i];
        if (bounding_rect.y >= object->bounding_rect.y - bounding_rect.h) {
            dxy[1] += object->bounding_rect.y - (bounding_rect.y + bounding_rect.h);
            dxy[1] = min(dxy[1], object->dxy[1]);
            bounding_rect.y = object->bounding_rect.y - bounding_rect.h;
            // set onBody
            int px_on_platform;
            if (object->bounding_rect.x + object->bounding_rect.w - (bounding_rect.x + bounding_rect.w) < 0)
                px_on_platform = min(object->bounding_rect.x + object->bounding_rect.w - bounding_rect.x, bounding_rect.w);
            else
                px_on_platform = min(bounding_rect.x + bounding_rect.w - object->bounding_rect.x , bounding_rect.w);

            if (px_on_platform >= max_px_on_platform) {
                max_px_on_platform = px_on_platform;
                on_body = object;
            }
            is_falling = false;
        }
    }
    // dynamic
    for (unsigned int i=0; i<dynamic_coll_bodies[BOTTOM].size(); i++) {
        DynamicBody* object = dynamic_coll_bodies[BOTTOM][i];
        if (bounding_rect.y >= object->bounding_rect.y - bounding_rect.h) {
            dxy[1] += object->bounding_rect.y - (bounding_rect.y + bounding_rect.h);
            dxy[1] = min(dxy[1], object->dxy[1]);

            bounding_rect.y = object->bounding_rect.y - bounding_rect.h;
            // set onBody
            int px_on_platform;
            if (object->bounding_rect.x + object->bounding_rect.w - (bounding_rect.x + bounding_rect.w) < 0)
                px_on_platform = min(object->bounding_rect.x + object->bounding_rect.w - bounding_rect.x, bounding_rect.w);
            else
                px_on_platform = min(bounding_rect.x + bounding_rect.w - object->bounding_rect.x , bounding_rect.w);

            if (px_on_platform >= max_px_on_platform) {
                max_px_on_platform = px_on_platform;
                on_body = object;
            }
            is_falling = false;
        }
    }
    updateCollisions(static_bodies, dynamic_bodies);

    if (is_falling)
        on_body = nullptr;
}


void DynamicBody::applyNewtonLaw()
{
    if (on_body == nullptr) {
        addForce(mass*GRAVITY[0], mass*GRAVITY[1]); // gravity
        addForce(-dxy[0] * air_friction_coefficient, 0); // air friction
    }
    else {
        addForce(-dxy[0] * on_body->friction_coefficient, 0); // ground friction
    }

    // Newtons's 3rd Law
    dxy[0] += PIXELS_NB_PER_METER * pow(NOMINAL_DT, 2) / mass * (force[0] + collision_force[0]);
    dxy[1] += PIXELS_NB_PER_METER * pow(NOMINAL_DT, 2) / mass * (force[1] + collision_force[1]);

    // limit dxy to max_dxy
    if (dxy[0] > 0)
        dxy[0] = min(current_max_dxy[0], dxy[0]);
    else if (dxy[0] < 0)
        dxy[0] = max(-current_max_dxy[0], dxy[0]);

    if (dxy[1] > 0)
        dxy[1] = min(current_max_dxy[1], dxy[1]);
    else if (dxy[1] < 0)
        dxy[1] = max(-current_max_dxy[1], dxy[1]);

    bounding_rect.x += round(dxy[0]);
    bounding_rect.y += round(dxy[1]);

    collision_force[0] = .0;
    collision_force[1] = .0;

    if (on_body != nullptr)
        bounding_rect.x += round(getAbsolutePlatformDx());
}



float DynamicBody::getAbsolutePlatformDx()
{
    float dx = .0;
    Body* platform = on_body;

    while (platform != nullptr)
    {
        dx += platform->dxy[0];
        platform = platform->getOnBody();
    }
    return dx;
}

float DynamicBody::getAbsolutePlatformDy()
{
    float dy = .0;
    Body* platform = on_body;

    while (platform != nullptr)
    {
        dy += platform->dxy[1];
        platform = platform->getOnBody();
    }
    return dy;
}

float DynamicBody::getEquivalentMass()
{
    float eq_mass = mass;
    vector<DynamicBody*> top_dynamic_bodies = getTopDynamicBodies();

    for (unsigned int i=0; i<top_dynamic_bodies.size(); i++)
        eq_mass += top_dynamic_bodies[i]->mass;

    return eq_mass;
}


vector<DynamicBody*> DynamicBody::getTopDynamicBodies()
{
    vector<DynamicBody*> dynamic_bodies;

    //dynamic_bodies.push_back(this);
    for (unsigned int i=0; i<dynamic_coll_bodies[TOP].size(); i++) // for each object on this' top
    {
        DynamicBody* dynamic_body = dynamic_coll_bodies[TOP][i];
        if ( find(dynamic_bodies.begin(), dynamic_bodies.end(), dynamic_body) == dynamic_bodies.end() )
            dynamic_bodies.push_back(dynamic_body);

        vector<DynamicBody*> object_dynamic_bodies = dynamic_body->getTopDynamicBodies(); //get object's top objects

        for (unsigned int j=0; j<object_dynamic_bodies.size(); j++) // for each object in object's top objects
        {
            DynamicBody* object_dynamic_body = object_dynamic_bodies[j];
            // if object NOT in dynamic_bodies, it's added
            if ( find(dynamic_bodies.begin(), dynamic_bodies.end(), object_dynamic_body) == dynamic_bodies.end() )
                dynamic_bodies.push_back(object_dynamic_body);
        }
    }
    return dynamic_bodies;
}





