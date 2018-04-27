#include "game_object.hpp"

GameObject::GameObject(int x, int y, int w, int h, float life_time, float friction_coefficient, bool ghost_left, bool ghost_top, bool ghost_right, bool ghost_bottom) : Sprite(x, y, w, h), Body(x, y, w, h, friction_coefficient, ghost_left, ghost_top, ghost_right, ghost_bottom)
{
    creation_tick = SDL_GetTicks();
    this->life_time = life_time;
}

void GameObject::update()
{
    Sprite::update();
    Body::update();
    rect = bounding_rect;
    
    
    // end's life
    if (life_time !=0) {
        if (SDL_GetTicks() - creation_tick > 1000 * life_time) {
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = SDL_USEREVENT;
            event.user.code = USER_EVENT_END_OBJECT;
            event.user.data1 = this;
            event.user.data2 = 0;
            SDL_PushEvent(&event);
            life_time = 0;
        }
    }
}

void GameObject::init(SDL_Renderer *pt_renderer)
{
    Sprite::init(pt_renderer);
}

void GameObject::moveTo(int x, int y)
{
    Sprite::moveTo(x, y);
    Body::moveTo(x, y);
}

void GameObject::addObject(GameObject *object, int x, int y, float dx, float dy)
{
    // sets the object in this' referential
    object->bounding_rect.x = bounding_rect.x + x;
    object->bounding_rect.y = bounding_rect.y + y;
    object->dxy[0] = dx; //+ dxy[0];
    object->dxy[1] = dy; //+ dxy[1];
    
    
    // push ADD_OBJECT event
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = SDL_USEREVENT;
    event.user.code = USER_EVENT_ADD_OBJECT;
    event.user.data1 = object;
    event.user.data2 = this;
    SDL_PushEvent(&event);
}




