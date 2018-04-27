#include "box.hpp"

Box::Box(GameEngine* pt_game_engine, int x, int y) : DynamicGameObject(x, y, 32, 32), Body(x, y, 32, 32, 10, false, false, false, false), Sprite(x, y, 32, 32)
{
    pt_game_engine->addObject(this);
    
    // texture init
    SDL_DestroyTexture(texture);
    texture = pt_game_engine->graphics_engine->texture_manager->loadTexture("box");


};
