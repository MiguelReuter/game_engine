#ifndef ladder_hpp
#define ladder_hpp

#include <iostream>
#include <math.h>
#include <stdio.h>

#include "../src/game_engine.hpp"

using namespace std;

class Ladder : public StaticGameObject
{
public:
    Ladder(GameEngine* pt_game_engine, int x=0, int y=0, int w=64, int h=64);
};
#endif /* ladder_hpp */
