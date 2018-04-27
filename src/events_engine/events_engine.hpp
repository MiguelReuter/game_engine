#ifndef events_engine_hpp
#define events_engine_hpp

enum KEY_STATES {KEY_RELEASED=0,
    KEY_JUST_PRESSED=1,
    KEY_PRESSED=2,
    KEY_JUST_RELEASED=3
};

#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "user_events.h"
#include "../game_types/game_types.h"


using namespace std;

typedef struct
{
    char key[SDL_NUM_SCANCODES];
    int mouse_x, mouse_y;
    int mouse_xrel, mouse_yrel;
    char mouse_buttons[8];
    char quit;

    // user events
    vector<GameObject*> game_objects_to_delete;
    vector<GameObject*> game_objects_to_add;
} Input;



class EventsEngine
{
public:
    static EventsEngine* Instance();
    Input in;

    void update();
    void init();


private:
    EventsEngine();
    EventsEngine(EventsEngine const&);
    EventsEngine& operator=(EventsEngine const&);
    static EventsEngine* m_instance;
};










#endif /* events_engine_hpp */
