#include "character.hpp"

Character::Character(GameEngine* pt_game_engine, int x, int y, int w, int h, float friction_coefficient, float air_friction_coefficient, float mass, float max_vel_x, float max_vel_y, float max_height_jump) : GameObject(x, y, w, h, 0, friction_coefficient, false, false, false, false), DynamicBody(x, y, w, h, friction_coefficient, air_friction_coefficient, mass, max_vel_x, max_vel_y, false, false, false, false, PRIMARY), Body(x, y, w, h,friction_coefficient, false, false, false, false), AnimatedSprite(x, y, w, h), Sprite(x, y, w, h)
{
    pt_game_engine->addObject(this);

    // speed
    reduced_speed_factor = 2; // reduced speed factor when crouching, climbing a ladder...

    // jump
    this->max_height_jump = max_height_jump;
    this->dy_jump = -sqrtf(2 * GRAVITY[1] * max_height_jump * PIXELS_NB_PER_METER * NOMINAL_DT);

    // crouching
    standard_rect_size[0] = w;
    standard_rect_size[1] = h;

    crouching_rect_size[0] = w;
    crouching_rect_size[1] = h / 2;

    // climbing
    climbing_speed = 2;

    // sprite animations
    //vector<SDL_Texture*> sprites = pt_game_engine->graphics_engine->texture_manager->loadTexturesStack("duffy_sprite_sheet", 134, 140, 0, 0);
    vector<SDL_Texture*> sprites = pt_game_engine->graphics_engine->texture_manager->loadTexturesStack("character_sprite_sheet", 40, 48, 0, 0);
    animations = vector<SpriteAnimation*> (ANIMATIONS_MAX, nullptr);

    animations[CROUCHING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 24, sprites.begin() + 26), FLIPPER, 6);
    animations[RUNNING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 9, sprites.begin() + 17), LOOP_START, 6);
    animations[JUMPING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 19, sprites.begin() + 24), PLAY, 6);
    animations[FALLING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 23, sprites.begin() + 24), PLAY, 6);
    animations[TOUCHING_GROUND] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 24, sprites.begin() + 27), PLAY, 6);
    animations[IDLING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin(), sprites.begin() + 3), LOOP_START, 12);
    animations[HITTING] = new SpriteAnimation(vector<SDL_Texture*> (sprites.begin() + 27, sprites.begin() + 35), PLAY, 6);
}


void Character::move(MOVES move)
{
    if (move == NO_MOVE)
        is_moving = false;
    else {
        if (move == RIGHT_MOVE) {
            addForce(100, 0);
            flip = SDL_FLIP_NONE;
        }
        else {
            addForce(-100, 0);
            flip = SDL_FLIP_HORIZONTAL;
        }
        is_moving = true;
    }
}


void Character::jump()
{
    if (!is_falling) {
        dxy[1] += dy_jump;
        is_jumping = true;
    }
    is_climbing = false;
}

void Character::crouch()
{
    if (!is_crouching and !is_climbing) {
        // resize bounding rect
        modifyBoundingRect(bounding_rect.x, bounding_rect.y + standard_rect_size[1] - crouching_rect_size[1],
                           bounding_rect.w, crouching_rect_size[1]);
        is_crouching = true;
    }
}

void Character::stand_up()
{
    if (canMoveAtTop() and is_crouching) {
        // resize bounding rect
        modifyBoundingRect(bounding_rect.x, bounding_rect.y - standard_rect_size[1] + crouching_rect_size[1],
                           bounding_rect.w, standard_rect_size[1]);
        is_crouching = false;
    }
}

void Character::hit()
{
    cout << "HIT" << endl;
    is_hitting = true;
}

void Character::update()
{
    GameObject::update();

    // bricolage...
    rect.y = bounding_rect.h + bounding_rect.y - standard_rect_size[1];
    rect.h = standard_rect_size[1];


    current_max_dxy[0] = absolute_max_dxy[0];
    current_max_dxy[1] = absolute_max_dxy[1];

    is_touching_ladder = false;

    // interaction with ghost objects
    vector<Body*> ghosts_bodies = getGhostCollBodies();
    for (int i=0; i<ghosts_bodies.size(); i++) {
        // ladder
        Ladder* ladder = dynamic_cast<Ladder*>(ghosts_bodies[i]);
        if (ladder != nullptr)
            manageLadderBehaviour(ladder);

        // other object...


    }

    if (!is_touching_ladder) {
        is_climbing = false;
        touched_ladder = nullptr;
    }


    // reduce walk speed
    if (is_crouching or is_climbing)
        current_max_dxy[0] = absolute_max_dxy[0] / reduced_speed_factor;

    /* ---------- ANIMATIONS ----------*/
    playAnimation();
}

void Character::climb(bool rise){ // rise : true -> character rises, else he goes down
    if (is_touching_ladder and (on_body == nullptr or on_body == touched_ladder or rise)) {
        stand_up();
        is_falling = false;
        is_climbing = true;

        if (rise) { // character goes up
            if (touched_ladder->bounding_rect.y != bounding_rect.y + bounding_rect.h)
                dxy[1] = -climbing_speed;
            else
                is_climbing = false;
        }
        else // character goes down
            dxy[1] = climbing_speed;
    }
}

void Character::manageLadderBehaviour(Ladder *ladder)
{
    is_touching_ladder = true;
    touched_ladder = ladder;
    if (is_climbing) {
        if (on_body == nullptr) // if the character starts to climb the ladder (in the air or on the ground)
            on_body = ladder;
        else if (on_body != touched_ladder) // if character is on ladder but touches the ground
            is_climbing = false;
        is_falling = false;
        dxy[1] = 0;
    }
    else {
        // if the character falls on the top off the ladder
        if ((ladder->bounding_rect.y <= bounding_rect.y + bounding_rect.h) and (ladder->previous_bounding_rect.y >= previous_bounding_rect.y + previous_bounding_rect.h) and is_falling) {
            bounding_rect.y = ladder->bounding_rect.y - bounding_rect.h;
            dxy[1] = 0;
            is_falling = false;
            is_climbing = false;
            on_body = ladder;
        }
    }
}

void Character::playAnimation()
{





    // moving
    if (is_moving) {
        if (!is_falling)
            current_animation = animations[RUNNING];
        else
            current_animation = animations[JUMPING];
        current_animation->setPulseOn();
    }
    else if (current_animation == animations[RUNNING])
        current_animation = nullptr;

    // jumping
    if (is_jumping) {
        current_animation = animations[JUMPING];
        current_animation->setPulseOff();
        current_animation->setPulseOn();
        is_jumping = false;
    }

    // hitting
    if (is_hitting){
        current_animation = animations[HITTING];
        current_animation->setPulseOn();
        //cout << current_animation->getCurrentFrameIndex() << endl;
        if (animations[HITTING]->isEndReached())
        {
            is_hitting = false;
            current_animation->setPulseOff();
            current_animation = nullptr;
        }
    }


    // crouching
    if (is_crouching) {
        current_animation = animations[CROUCHING];
        current_animation->setPulseOn();
    }
    // stand up
    else {
        if (current_animation == animations[CROUCHING])
            current_animation->setPulseOff();
    }





    // touch the ground
    if ( (current_animation == animations[JUMPING] or current_animation == animations[FALLING]) and !is_falling and current_animation->isEndReached()) {
        current_animation = animations[TOUCHING_GROUND];
        current_animation->setPulseOff();
        current_animation->setPulseOn();
    }
    // idle after touching ground or crouching
    else if ( (current_animation == animations[TOUCHING_GROUND] or current_animation == animations[CROUCHING] ) and current_animation->isEndReached())
        current_animation = nullptr;
    //idle
    if (current_animation == nullptr or current_animation == animations[IDLING]) {
        if (is_falling)
            current_animation = animations[FALLING];
        else
            current_animation = animations[IDLING];
        current_animation->setPulseOn();
    }
    // playing the animation
    texture = current_animation->play();
}

