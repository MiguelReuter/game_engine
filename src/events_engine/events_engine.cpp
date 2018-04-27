#include "events_engine.hpp"


EventsEngine* EventsEngine::m_instance = nullptr;


EventsEngine* EventsEngine::Instance()
{
    if (!m_instance)
        m_instance = new EventsEngine();
    return m_instance;
}


EventsEngine::EventsEngine()
{
    //cout << "Events engine created" << endl;
}

void EventsEngine::update(){
    // update key status
    for (int i=0; i<SDL_NUM_SCANCODES; i++) {
        if (in.key[i] == KEY_JUST_PRESSED)
            in.key[i] = KEY_PRESSED;
        else if (in.key[i] == KEY_JUST_RELEASED)
            in.key[i] = KEY_RELEASED;
    }
    
    // update mouse buttons status
    for (int i=0; i<8; i++) {
        if (in.mouse_buttons[i] == KEY_JUST_PRESSED)
            in.mouse_buttons[i] = KEY_PRESSED;
        else if (in.mouse_buttons[i] == KEY_JUST_RELEASED)
            in.mouse_buttons[i] = KEY_RELEASED;
    }
    
    SDL_Event event;
    // for each event in the queue
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (in.key[event.key.keysym.scancode] != KEY_PRESSED)
                    in.key[event.key.keysym.scancode] = KEY_JUST_PRESSED;
                break;
                
            case SDL_KEYUP:
                in.key[event.key.keysym.scancode] = KEY_JUST_RELEASED;
                break;
                
            case SDL_MOUSEMOTION:
                in.mouse_x = event.motion.x;
                in.mouse_y = event.motion.y;
                in.mouse_xrel = event.motion.xrel;
                in.mouse_yrel = event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                in.mouse_buttons[event.button.button] = KEY_JUST_PRESSED;
                break;
            case SDL_MOUSEBUTTONUP:
                in.mouse_buttons[event.button.button] = KEY_JUST_RELEASED;
                break;
            
            case SDL_QUIT:
                in.quit = 1;
                break;
            
            case SDL_USEREVENT:
                if (event.user.code == USER_EVENT_END_OBJECT)
                    in.game_objects_to_delete.push_back(static_cast<GameObject*>(event.user.data1));
                else if (event.user.code == USER_EVENT_ADD_OBJECT)
                    in.game_objects_to_add.push_back(static_cast<GameObject*>(event.user.data1));
                
            default:
                break;
        }
    }
}

void EventsEngine::init()
{
    memset(&in, 0, sizeof(in));
}
