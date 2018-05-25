#include "circle_dynamic_body.hpp"





CircleDynamicBody::CircleDynamicBody(int x, int y, int diameter, float friction_coefficient, float air_friction_coefficient, float mass, float max_vel_x, float max_vel_y, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom) :  DynamicBody(x, y, diameter, diameter, friction_coefficient, air_friction_coefficient, mass, max_vel_x, max_vel_y, ghost_left, ghost_top, ghost_right, ghost_bottom){}

bool CircleDynamicBody::intersectCircleRect(const SDL_Rect* A_circle, const SDL_Rect* B, SDL_Point* result, int* intersect_type)
{
    /*
     intersect_type :
     SIDES :
        left: 0, top: 1, right: 2, bottom: 3
     CORNERS:
        left-bottom: 4, left-top: 5, right-top: 6, right-bottom: 7
     else: -1
    */


    SDL_Rect rects_result;//, coll_rect;
    /*coll_rect.x = A_circle->x - 1; y = bounding_rect.y - 1;
    coll_rect.w = bounding_rect.w + 2; coll_rect.h = bounding_rect.h + 2;*/

    if (SDL_IntersectRect(A_circle, B, &rects_result) == SDL_TRUE) {
        float radius = A_circle->w / 2.0;
        SDL_Point center;
        center.x = A_circle->w / 2 + A_circle->x; center.y = A_circle->h / 2 + A_circle->y;

        // ---------- SIDE COLLISIONS ----------
        // check for bottom and top side collisions
        if ( B->x <= center.x and center.x <= B->x + B->w) {
            result->x = center.x;
            // bottom collision
            if (rects_result.y + rects_result.h == A_circle->y + A_circle->h) {
                result->y = B->y;
                *intersect_type = BOTTOM_INTERSECT;
            }
            // top collision
            else {
                result->y = B->y + B->h;
                *intersect_type = TOP_INTERSECT;
            }

            return true;
        }
        // check for left and right side collisions
        if ( B->y <= center.y and center.y <= B->y + B->h) {
            result->y = center.y;
            // right collision
            if (rects_result.x + rects_result.w == A_circle->x + A_circle->w) {
                result->x = B->x;
                *intersect_type = RIGHT_INTERSECT;
            }

            // left collision
            else {
                result->x = B->x + B->w;
                *intersect_type = LEFT_INTERSECT;
            }

            return true;
        }

        // ---------- CORNER COLLISIONS ----------
        // top left corner
        if ( pow(center.x - B->x, 2) + pow(center.y - B->y, 2) <= pow(radius, 2) ) {
            result->x = B->x; result->y = B->y;
            *intersect_type = TOP_LEFT_INTERSECT;
            return true;
        }
        // top right corner
        if ( pow(center.x - (B->x + B->w), 2) + pow(center.y - B->y, 2) <= pow(radius, 2) ) {
            result->x = B->x + B->w; result->y = B->y;
            *intersect_type = TOP_RIGHT_INTERSECT;
            return true;
        }
        // bottom left corner
        if ( pow(center.x - B->x, 2) + pow(center.y - (B->y + B->h), 2) <= pow(radius, 2) ) {
            result->x = B->x; result->y = B->y + B->h;
            *intersect_type = BOTTOM_LEFT_INTERSECT;
            return true;
        }
        // bottom right corner
        if ( pow(center.x - (B->x + B->w), 2) + pow(center.y - (B->y + B->h), 2) <= pow(radius, 2) ) {
            result->x = B->x + B->w; result->y = B->y + B->h;
            *intersect_type = BOTTOM_RIGHT_INTERSECT;
            return true;
        }
    }
    *intersect_type = -1;
    return false;
}


void CircleDynamicBody::update(vector<StaticBody *> static_bodies, vector<DynamicBody *> dynamic_bodies)
{
    applyNewtonLaw();
    resolveCollisions(static_bodies, dynamic_bodies);
    previous_bounding_rect = bounding_rect;
}

void CircleDynamicBody::updateCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies)
{
    SDL_Rect coll_rect;

    coll_rect.x = bounding_rect.x - 1;
    coll_rect.y = bounding_rect.y - 1;
    coll_rect.w = bounding_rect.w + 2;
    coll_rect.h = bounding_rect.h + 2;

    SDL_Point center;
    center.x = coll_rect.w / 2 + coll_rect.x;
    center.y = coll_rect.h / 2 + coll_rect.y;

    // ---------- STATIC COLLISIONS ----------
    for (unsigned int i=0; i<4; i++)
        static_coll_bodies[i].clear();
    for (unsigned int i=0; i<static_bodies.size(); i++) {
        StaticBody* object = static_bodies[i];
        SDL_Rect* obj_rect = &object->bounding_rect;

        SDL_Point collision_point;
        int* intersect_type = new int;

        if (intersectCircleRect(&bounding_rect, obj_rect, &collision_point, intersect_type) == true) {
            // bottom collision
            if ( !ghost_sides[BOTTOM] and !object->ghost_sides[TOP] ) {
                if ( dxy[1] - object->dxy[1] > 0 and *intersect_type != LEFT_INTERSECT and *intersect_type != RIGHT_INTERSECT )
                {
                    static_coll_bodies[BOTTOM].push_back(object);
                }
            }

            // top collision
            if ( !ghost_sides[TOP] and !object->ghost_sides[BOTTOM] ) {
                if ( -(collision_point.y - center.y) >= abs(collision_point.x - center.x) and dxy[1] - object->dxy[1] < 0 )
                    static_coll_bodies[TOP].push_back(object);
            }

            // right collision
            if ( !ghost_sides[RIGHT] and !object->ghost_sides[LEFT] ) {
                if ( collision_point.x - center.x >= abs(collision_point.y - center.y) and dxy[0] - object->dxy[0] > 0 )
                    static_coll_bodies[RIGHT].push_back(object);
            }

            // left collision
            if ( !ghost_sides[LEFT] and !object->ghost_sides[RIGHT] ) {
                if ( - (collision_point.x - center.x) >= abs(collision_point.y - center.y) and dxy[0] - object->dxy[0] < 0 )
                    static_coll_bodies[LEFT].push_back(object);
            }
        }
        delete intersect_type;
    }


    // ---------- DYNAMIC COLLISIONS ----------
    for (unsigned int i=0; i<4; i++)
        dynamic_coll_bodies[i].clear();
    for (unsigned int i=0; i<static_bodies.size(); i++) {
        DynamicBody* object = dynamic_bodies[i];
        SDL_Rect* obj_rect = &object->bounding_rect;

        SDL_Point collision_point;
        int* intersect_type = new int;

        if (intersectCircleRect(&bounding_rect, obj_rect, &collision_point, intersect_type) == true) {
            // bottom collision
            if ( !ghost_sides[BOTTOM] and !object->ghost_sides[TOP] ) {
                if ( dxy[1] - object->dxy[1] > 0 and *intersect_type != LEFT_INTERSECT and *intersect_type != RIGHT_INTERSECT )
                {
                    dynamic_coll_bodies[BOTTOM].push_back(object);
                }
            }

            // top collision
            if ( !ghost_sides[TOP] and !object->ghost_sides[BOTTOM] ) {
                if ( -(collision_point.y - center.y) >= abs(collision_point.x - center.x) and dxy[1] - object->dxy[1] < 0 )
                    dynamic_coll_bodies[TOP].push_back(object);
            }

            // right collision
            if ( !ghost_sides[RIGHT] and !object->ghost_sides[LEFT] ) {
                if ( collision_point.x - center.x >= abs(collision_point.y - center.y) and dxy[0] - object->dxy[0] > 0 )
                    dynamic_coll_bodies[RIGHT].push_back(object);
            }

            // left collision
            if ( !ghost_sides[LEFT] and !object->ghost_sides[RIGHT] ) {
                if ( - (collision_point.x - center.x) >= abs(collision_point.y - center.y) and dxy[0] - object->dxy[0] < 0 )
                    dynamic_coll_bodies[LEFT].push_back(object);
            }
        }
        delete intersect_type;
    }
}


void CircleDynamicBody::resolveCollisions(vector<StaticBody*> static_bodies, vector<DynamicBody*> dynamic_bodies)
{

    // idée : si collision avec coin haut gauche d'un rect, on considère 2 collisions et pas une seule. --> chaque collision est gérée en déplaçant le cercle sur uniquement un axe.


    updateCollisions(static_bodies, dynamic_bodies);
    is_falling = true;

    int radius = bounding_rect.w / 2;

    for (unsigned int i=0; i<static_coll_bodies[BOTTOM].size(); i++)
    {
        Body* object = static_coll_bodies[BOTTOM][i];
        SDL_Point collision_point;
        int* intersect_type = new int;

        SDL_Point center;
        center.x = bounding_rect.w / 2 + bounding_rect.x;
        center.y = bounding_rect.h / 2 +
        bounding_rect.y;

        if ( intersectCircleRect(&bounding_rect, &object->bounding_rect, &collision_point, intersect_type) )
        {
            // process dx and dy (the circle is moved appart on the line center/collision point)
            float MC = sqrt(pow(center.x - collision_point.x, 2) + pow(center.y - collision_point.y, 2));
            float dx = 0, dy = 0;

            if ( MC != 0 and MC < radius ) {
                dx = (radius / MC - 1) * (center.x - collision_point.x);
                dy = (radius / MC - 1) * (center.y - collision_point.y);
            }

            // if circle is on the ground
            if ( object->bounding_rect.x <= center.x and center.x <= object->bounding_rect.x + object->bounding_rect.w ) {
                on_body = object;
                is_falling = false;

                dy = object->bounding_rect.y - (bounding_rect.y + bounding_rect.h);
                dxy[1] += dy;
                dxy[1] = min(dxy[1], object->dxy[1]);

                bounding_rect.y = object->bounding_rect.y - bounding_rect.h;
            }
            else if (!is_falling)
                dy = 0;
            else {
                dxy[1] += dy;
                bounding_rect.y += dy;
            }
        }
        delete intersect_type;
    }



    /*
    vector<Body*> bodies;
    // dynamic collision
    for(int i=0; i<dynamic_coll_bodies[LEFT].size(); i++)
        bodies.push_back(dynamic_coll_bodies[LEFT][i]);
    for(int i=0; i<dynamic_coll_bodies[RIGHT].size(); i++)
        bodies.push_back(dynamic_coll_bodies[RIGHT][i]);
    for(int i=0; i<dynamic_coll_bodies[TOP].size(); i++)
        bodies.push_back(dynamic_coll_bodies[TOP][i]);
    for(int i=0; i<dynamic_coll_bodies[BOTTOM].size(); i++)
        bodies.push_back(dynamic_coll_bodies[BOTTOM][i]);

    // static collision
    for(int i=0; i<static_coll_bodies[LEFT].size(); i++)
        bodies.push_back(static_coll_bodies[LEFT][i]);
    for(int i=0; i<static_coll_bodies[RIGHT].size(); i++)
        bodies.push_back(static_coll_bodies[RIGHT][i]);
    for(int i=0; i<static_coll_bodies[TOP].size(); i++)
        bodies.push_back(static_coll_bodies[TOP][i]);
    for(int i=0; i<static_coll_bodies[BOTTOM].size(); i++)
        bodies.push_back(static_coll_bodies[BOTTOM][i]);


    int radius = bounding_rect.w / 2;

    for (int i=0; i<bodies.size(); i++)
    {
        Body* object = bodies[i];
        SDL_Point collision_point;

        SDL_Point center;
        center.x = bounding_rect.w / 2 + bounding_rect.x;
        center.y = bounding_rect.h / 2 +
        bounding_rect.y;

        if ( intersectCircleRect(&bounding_rect, &object->bounding_rect, &collision_point) ) {
            // process dx and dy (the circle is moved appart on the line center/collision point)
            float MC = sqrt(pow(center.x - collision_point.x, 2) + pow(center.y - collision_point.y, 2));
            float dx = 0, dy = 0;

            if ( MC != 0 and MC < radius ) {
                dx = (radius / MC - 1) * (center.x - collision_point.x);
                dy = (radius / MC - 1) * (center.y - collision_point.y);
            }

            // if circle is on the ground
            if ( object->bounding_rect.x <= center.x and center.x <= object->bounding_rect.x + object->bounding_rect.w and object->bounding_rect.y > center.y) {
                onBody = object;
                is_falling = false;

                dy = object->bounding_rect.y - (bounding_rect.y + bounding_rect.h);
                dxy[1] += dy;
                dxy[1] = min(dxy[1], object->dxy[1]);

                bounding_rect.y = object->bounding_rect.y - bounding_rect.h;
            }
            else if (!is_falling)
                dy = 0;
            else {
                dxy[1] += dy;
                bounding_rect.y += dy;
            }


            if (dx > 0 and canMoveAtRight()) {
                dxy[0] += dx;
                bounding_rect.x += dx;
            }
            else if (dx < 0 and canMoveAtLeft()) {
                dxy[0] += dx;
                bounding_rect.x += dx;
            }
            updateCollisions(static_bodies, dynamic_bodies);
        }
    }
    */


    if (is_falling)
        on_body = nullptr;

}


void CircleDynamicBody::applyNewtonLaw()
{
    if (on_body == nullptr) {
        addForce(mass*GRAVITY[0], mass*GRAVITY[1]); // gravity
        addForce(-dxy[0] * air_friction_coefficient, 0); // air friction
    }
    else
        addForce(- 0.05 * dxy[0] * on_body->friction_coefficient, 0); // ground friction

    // Newtons's 3rd Law
    dxy[0] += PIXELS_NB_PER_METER * pow(NOMINAL_DT, 2) / mass * (force[0] + collision_force[0]);
    dxy[1] += PIXELS_NB_PER_METER * pow(NOMINAL_DT, 2) / mass * (force[1] + collision_force[1]);

    // limit dxy to current_max_dxy
    if (dxy[0] > 0)
        dxy[0] = min(current_max_dxy[0], dxy[0]);
    else if (dxy[0] < 0)
        dxy[0] = max(-current_max_dxy[0], dxy[0]);

    if (dxy[1] > 0)
        dxy[1] = min(current_max_dxy[1], dxy[1]);
    else if (dxy[1] < 0)
        dxy[1] = max(-current_max_dxy[1], dxy[1]);

    // reset forces
    force[0] = .0; force[1] = .0;
    collision_force[0] = .0; collision_force[1] = .0;

    bounding_rect.x += round(dxy[0]);
    bounding_rect.y += round(dxy[1]);

    if (on_body != nullptr)
        bounding_rect.x += on_body->dxy[0];
}


