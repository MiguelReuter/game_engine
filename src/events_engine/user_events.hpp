#ifndef user_events_hpp
#define user_events_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>


using namespace std;

class UserEvents
{
public:
    static UserEvents* Instance();
    
    
    Uint32 USER_EVENT_END_OBJECT;
    
    
private:
    UserEvents();
    UserEvents(UserEvents const&);
    UserEvents& operator=(UserEvents const&);
    static UserEvents* m_instance;
};


#endif /* user_events_hpp */
