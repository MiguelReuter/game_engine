#ifndef armature_hpp
#define armature_hpp

#include <iostream>
#include <map>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "bone.hpp"

using namespace std;

class Bone;

class Armature
{
public:
    
    // bones
    SDL_Point getBonePosition(string bone);
    float getBoneAngle(string bone);
    float getBoneLength(string bone);
    void setBoneLocalPosition(string bone, SDL_Point point);
    void setBoneLocalAngle(string bone, float angle);
    
    void setBoneGlobalPosition(string bone, SDL_Point point);
    void setBoneGlobalAngle(string bone, float angle);
    
    void getExtremePointsReachable(SDL_Point* top_left_corner, SDL_Point* bottom_right);
    
    //set bone attributes
    Armature(SDL_Point origin, map<string, Bone*> bones);
    Armature(SDL_Point origin);
    
    map<string, Bone*> bones;
    
    void init(SDL_Renderer* pt_renderer);
    ~Armature();
    
private:
    SDL_Point origin;
};




#endif /* armature_hpp */
