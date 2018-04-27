#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

#include "../game_types/game_types.h"
#include "graphics_engine.hpp"
#include "../constants.h"


class GraphicsEngine;
//class GameObject;

using namespace std;

class Camera
{
public:
    SDL_Renderer* renderer = nullptr;
    GraphicsEngine* graphics_engine = nullptr;

    // pursuit
    void setFocusOn(GameObject* pt_object_focused){object_focused = pt_object_focused;};
    GameObject* getFocusOn(){return object_focused;};
    GameObject* object_focused = nullptr;
    int pxl_offset_rect_focused[2]; // offset in pixels between camera center and focused rect center for each axis

    void setPursuitThreshold(int th_x, int th_y){
        pursuit_threshold[0] = max(th_x, 0);
        pursuit_threshold[1] = max(th_y, 0);};

    // camera properties
    SDL_Rect rect;

    float getZoom(){return zoom;}
    void setZoom(float z);

    void update();
    void moveTo(int x, int y);
    void moveCenterTo(int x, int y);

    int getXCenter(){return rect.x + rect.w / 2;};
    int getYCenter(){return rect.y + rect.h / 2;};

    void deleteObject(GameObject* object);

    bool isCurrentCamera();

    Camera(GraphicsEngine* pt_graphics_engine, SDL_Renderer* pt_renderer,
           int center_x, int center_y, float zoom, float pursuit_speed);

private:
    // pursuit attributes
    int pursuit_threshold[2]; // in logical pixels in each axis
    float pursuit_speed; // to normalize with fps
    // camera attributes
    float x, y; // float coords to allow slow moves
    float zoom;
};






#endif /* camera_hpp */
