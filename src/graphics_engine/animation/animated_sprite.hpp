#ifndef animated_sprite_hpp
#define animated_sprite_hpp

#include <stdio.h>

#include "../sprite.hpp"
#include "sprite_animation.hpp"

class AnimatedSprite : public virtual Sprite
{
public:
    AnimatedSprite(int x, int y, int w, int h);

protected:
    vector<SpriteAnimation*> animations;
    SpriteAnimation* current_animation = nullptr;
    void playAnimation();

};


#endif /* animated_sprite_hpp */
