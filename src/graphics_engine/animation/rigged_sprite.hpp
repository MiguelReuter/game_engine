#ifndef armature_sprite_hpp
#define armature_sprite_hpp

#include <map>
#include <algorithm>
#include <stdio.h>
#include <string>


#include "armature.hpp"
#include "../sprite.hpp"

using namespace std;

class RiggedSprite : public virtual Sprite
{
public:
    RiggedSprite(int x, int y, int w, int h);
    Armature* armature=nullptr;
    void init(SDL_Renderer* pt_renderer);
    void update();

    SDL_Point delta_origin;

    ~RiggedSprite();

};


#endif /* armature_sprite_hpp */
