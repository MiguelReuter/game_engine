#ifndef sprite_animation_hpp
#define sprite_animation_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <algorithm>

#include <vector>

enum ANIMATION_MODE {
    PLAY = 0, // play animation once from start to end when pulse = true
    REVERSED = 1, // play animation once from end to start when pulse = true
    PING_PONG = 2, // play animation once from start to end, then end to start ...
    FLIPPER = 3, // play animation once from start to end if pulse = true. When pulse = false, end to start animation.
    LOOP_END = 4, // end to start loop when pulse = true
    LOOP_START = 5, // start to end loop when pulse = true
    PROPERTY = 6 // frame displayed = frame parameter value in play(frame)
};




using namespace std;

class SpriteAnimation
{
public:
    SpriteAnimation(vector<SDL_Texture*> pt_textures, ANIMATION_MODE mode, int rate = 1, bool repeat=false);
    
    //SDL_Texture* reset();
    SDL_Texture* play(int frame = 0);
    
    int getCurrentFrameIndex(){return current_frame_index;};
    bool getPulse(){return pulse;};
    bool getTrigger(){return trigger;};
    bool isEndReached(){return end_reached;};
    
    void setPulseOn();
    void setPulseOff();
    ANIMATION_MODE getAnimationMode(){return mode;};
    
private:
    bool pulse = false;
    bool trigger = false;
    bool repeat = false;
    
    bool end_reached = false;
    
    vector<SDL_Texture*> textures;
    ANIMATION_MODE mode;
    int rate, current_fps_in_rate;
    int current_frame_index;
    int frames_nb;
    int count = 0;
    bool start_to_end = true;
};






#endif /* sprite_animation_hpp */
