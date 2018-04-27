#ifndef user_events_h
#define user_events_h

enum USER_EVENTS {
    USER_EVENT_END_OBJECT, // data1 : GameObject* to delete
    USER_EVENT_ADD_OBJECT, // data1 : GameObject* to add
};

#endif /* user_events_h */
