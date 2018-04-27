#ifndef bone_hpp
#define bone_hpp

#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "../sprite.hpp"
#include "animated_sprite.hpp"
//#include "armature_sprite.hpp"
#include "armature.hpp"

#define PI 3.14159265

using namespace std;

class Bone
{
    friend class Armature;
public:
    Bone(int global_x, int global_y, float global_angle, float length);
    Bone(int local_x, int local_y, float local_angle, float length, Bone* parent);
    Sprite* sprite=nullptr;

private:
    // ---- local attributes ----
    // head position
    void setLocalHeadPosition(int x, int y);
    void setLocalHeadPosition(SDL_Point pt){setLocalHeadPosition(pt.x, pt.y);};
    SDL_Point getLocalHeadPosition(){return local_head;};
    // tail position
    SDL_Point getLocalTailPosition(){return local_tail;};
    // angle
    void setLocalAngle(float angle);
    float getLocalAngle(){return local_angle;};

    // ---- global attributes ----
    // head position
    void setGlobalHeadPosition(int x, int y);
    void setGlobalHeadPosition(SDL_Point pt){setGlobalHeadPosition(pt.x, pt.y);};
    SDL_Point getGlobalHeadPosition(){return global_head;};
    // tail position
    SDL_Point getGlobalTailPosition(){return global_tail;};
    // angle
    void setGlobalAngle(float angle);
    float getGlobalAngle(){return global_angle;};

    // ---- absolute attributes ----
    /*
     if parent :
        absolute = absolute parent's attributes + local attributes in parent's reference
     else :
        absolute = global attributes
     */
    SDL_Point getAbsoluteHeadPosition();
    SDL_Point getAbsoluteTailPosition();
    float getAbsoluteAngle();



    SDL_Point global_head, global_tail; // armature's center relative
    float global_angle; // armature relative

    SDL_Point local_head, local_tail;
    float local_angle;

    float length;

    Bone* parent=nullptr;
    vector<Bone*> children;
    ~Bone();
};



#endif /* bone_hpp */
