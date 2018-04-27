#ifndef rigged_test_hpp
#define rigged_test_hpp

#include <iostream>
#include <map>
#include <math.h>
#include <stdio.h>

#include "../src/game_engine.hpp"

class Riggedtest : public GameObject, public RiggedSprite
{
public:
    void update();
    Riggedtest(GameEngine* pt_game_engine, int x, int y);
    void init(SDL_Renderer* pt_renderer);
    //void moveTo();

    int i = 0; // test pour anim
};




#endif /* rigged_test_hpp */
