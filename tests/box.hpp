#ifndef box_hpp
#define box_hpp

#include <iostream>
#include <stdio.h>

#include "../src/game_engine.hpp"

using namespace std;


class Box : public DynamicGameObject
{
public:
    Box(GameEngine* pt_game_engine, int x, int y);
};

#endif /* box_hpp */
