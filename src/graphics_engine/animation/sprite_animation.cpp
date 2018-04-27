#include "sprite_animation.hpp"

SpriteAnimation::SpriteAnimation(vector<SDL_Texture*> pt_textures, ANIMATION_MODE anim_mode, int i_rate, bool b_repeat)
{
    textures = pt_textures;
    frames_nb = int(textures.size());
    mode = anim_mode;
    rate = i_rate;
    current_fps_in_rate = 0;
    repeat = b_repeat;
};

SDL_Texture* SpriteAnimation::play(int frame)
{
    if(current_fps_in_rate == 0)
    {
        switch (mode) {
            case PLAY:
                if (pulse and trigger)
                    current_frame_index = 0;
                else if (pulse) {
                    current_frame_index = min(current_frame_index + 1, frames_nb - 1);
                    if (current_frame_index == frames_nb - 1)
                        end_reached = true;
                }
                else
                    current_frame_index = 0;
                break;

            case REVERSED:
                if (pulse and trigger)
                    current_frame_index = frames_nb - 1;
                else if (pulse) {
                    current_frame_index = max(current_frame_index - 1, 0);
                    end_reached = true;
                }
                else
                    current_frame_index = frames_nb - 1;
                break;

            case PING_PONG:
                if (pulse and trigger) {
                    start_to_end = true;
                    current_frame_index = 0;
                }
                else if (pulse and !(!repeat and count!= 0)) {
                    if (start_to_end) {
                        current_frame_index = min(current_frame_index + 1, frames_nb - 1);
                        if (current_frame_index == frames_nb - 1)
                            start_to_end = false;
                    }
                    else {
                        current_frame_index = max(0, current_frame_index - 1);
                        if (current_frame_index == 0) {
                            count += 1;
                            start_to_end = true;
                        }
                    }
                }
                else {
                    current_frame_index = 0;
                    end_reached = true;
                }
                break;

            case FLIPPER:
                if (pulse and trigger)
                    current_frame_index = 0;
                else if (pulse)
                    current_frame_index = min(current_frame_index + 1, frames_nb - 1);
                else {
                    current_frame_index = max(current_frame_index - 1, 0);
                    if (current_frame_index == 0)
                        end_reached = true;
                }
                break;

            case LOOP_END:
                if (pulse and trigger)
                    current_frame_index = frames_nb - 1;
                else if (pulse) {
                    current_frame_index = (current_frame_index - 1) % frames_nb;
                    if (current_frame_index == -1)
                        current_frame_index = frames_nb - 1;
                }
                else
                    current_frame_index = frames_nb - 1;
                break;

            case LOOP_START:
                if (pulse and trigger)
                    current_frame_index = 0;
                else if (pulse)
                    current_frame_index = (current_frame_index + 1) % frames_nb;
                else
                    current_frame_index = 0;
                break;


            case PROPERTY:
                current_frame_index = max(0, min(frames_nb - 1, frame));
                break;

            default:
                current_frame_index = 0;
                break;
        }
        trigger = false;
    }

    current_fps_in_rate = (current_fps_in_rate + 1) % rate;

    return textures[current_frame_index];
};




void SpriteAnimation::setPulseOn()
{
    if (!pulse) {
        pulse = true;
        trigger = true;
        count = 0;
        current_fps_in_rate = 0;
        end_reached = false;
    }
}

void SpriteAnimation::setPulseOff()
{
    if (pulse) {
        pulse = false;
        trigger = true;
    }
}
