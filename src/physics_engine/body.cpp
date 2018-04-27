#include "body.hpp"

using namespace std;

Body::Body(int x, int y, int w, int h, float friction_coefficient, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom)
{
    this->friction_coefficient = friction_coefficient;

    ghost_sides[LEFT] = ghost_left;
    ghost_sides[TOP] = ghost_top;
    ghost_sides[RIGHT] = ghost_right;
    ghost_sides[BOTTOM] = ghost_bottom;

    ghost_for_primary_dynamic_object = false;

    dxy[0] = 0;
    dxy[1] = 0;

    // collisions attributes
    bounding_rect.x = x;
    bounding_rect.y = y;
    bounding_rect.w = w;
    bounding_rect.h = h;

    previous_bounding_rect = bounding_rect;
}



void Body::update(){
    applyModificationsOnBoundingRect();
}

void Body::moveTo(int x, int y)
{
    dxy[0] = 0;
    dxy[1] = 0;

    bounding_rect.x = x;
    bounding_rect.y = y;
}


void Body::move(float dx, float dy)
{
    previous_bounding_rect = bounding_rect;

    bounding_rect.x += round(dx);
    bounding_rect.y += round(dy);
}

void Body::modifyBoundingRect(int x, int y, int w, int h, bool modifs_on_previous_bounding_rect)
{
    bounding_rect_modifs[0] = x;
    bounding_rect_modifs[1] = y;
    bounding_rect_modifs[2] = w;
    bounding_rect_modifs[3] = h;

    this->modifs_on_previous_bounding_rect = modifs_on_previous_bounding_rect;
    has_to_modify = true;
}

void Body::applyModificationsOnBoundingRect()
{
    if (has_to_modify) {
        bounding_rect.x = bounding_rect_modifs[0];
        bounding_rect.y = bounding_rect_modifs[1];
        bounding_rect.w = bounding_rect_modifs[2];
        bounding_rect.h = bounding_rect_modifs[3];

        if (modifs_on_previous_bounding_rect)
            previous_bounding_rect = bounding_rect;

        has_to_modify = false;
    }
}
