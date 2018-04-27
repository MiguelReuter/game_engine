#ifndef character_hpp
#define character_hpp

#include <iostream>
#include <math.h>
#include <stdio.h>

#include "../src/game_engine.hpp"
#include "ladder.hpp"

using namespace std;


enum MOVES {
    LEFT_MOVE,
    RIGHT_MOVE,
    NO_MOVE
};

class Character : public GameObject, public DynamicBody, public AnimatedSprite
{
public:
    Character(GameEngine* pt_game_engine, int x=0, int y=0, int w=32, int h=64, float friction_coefficient=10, float air_friction_coefficient=10, float mass=1.0, float max_vel_x=5, float max_vel_y=10, float max_height_jump=1.5);

    void update();

    void jump();

    void crouch();
    void stand_up();

    void climb(bool rise);

    void hit();

    void move(MOVES move = NO_MOVE);



private:
    // move
    bool is_moving = false;

    // speed
    float reduced_speed_factor;

    // jump
    float max_height_jump; // in meters
    float dy_jump; //in pixels
    bool is_jumping = false;

    // crouch
    bool is_crouching = false;
    int crouching_rect_size[2], standard_rect_size[2];

    // climb
    float climbing_speed;
    bool is_climbing = false;
    bool is_touching_ladder = false;
    Ladder* touched_ladder = nullptr;
    void manageLadderBehaviour(Ladder* ladder);

    // hit
    bool is_hitting = false;

    // animations
    enum ANIMATIONS {
        CROUCHING,
        RUNNING,
        JUMPING,
        FALLING,
        TOUCHING_GROUND,
        IDLING,
        HITTING,
        ANIMATIONS_MAX
    };
    void playAnimation();
};


#endif /* character_hpp */
