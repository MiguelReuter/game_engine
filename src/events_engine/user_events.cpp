#include "user_events.hpp"

UserEvents* UserEvents::m_instance = nullptr;


UserEvents* UserEvents::Instance()
{
    if (!m_instance)
        m_instance = new UserEvents();
    return m_instance;
}


UserEvents::UserEvents()
{
    cout << "User events created" << endl;
    USER_EVENT_END_OBJECT = SDL_RegisterEvents(1);
    
    
    cout << "user event = " << USER_EVENT_END_OBJECT << endl;
}
