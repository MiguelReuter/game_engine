#ifndef body_hpp
#define body_hpp

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "../constants.h"


using namespace std;

class Body
{
public:
    SDL_Rect bounding_rect, previous_bounding_rect;

    // physics attributes
    float friction_coefficient;
    float dxy[2];
    bool ghost_sides[4];


    virtual void update();
    virtual void moveTo(int x, int y);
    virtual void move(float dx, float dy);

    bool ghost_for_primary_dynamic_object;
    virtual void setGhostForPrimaryDynamicBody(bool val=true){ghost_for_primary_dynamic_object = val;};

    virtual Body* getOnBody(){return on_body;};
    virtual void setOnBody(Body* body){on_body = body;};

    Body(int x=0, int y=0, int w=0, int h=0, float friction_coefficient=10, bool ghost_left=false, bool ghost_top=false, bool ghost_right=false, bool ghost_bottom=false);
    virtual ~Body(){};


protected:
    Body* on_body = NULL;

    // bounding rect modifications
    void modifyBoundingRect(int x, int y, int w, int h, bool modifs_on_previous_bounding_rect=false);
    void applyModificationsOnBoundingRect();
    int bounding_rect_modifs[4]; // x, y, w, h
    bool has_to_modify = false, modifs_on_previous_bounding_rect = false;


};

#endif /* body_hpp */
