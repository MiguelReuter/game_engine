#include "camera.hpp"



using namespace std;

Camera::Camera(GraphicsEngine* pt_graphics_engine, SDL_Renderer* pt_renderer, int center_x, int center_y, float z, float pursuit_speed)
{
    graphics_engine = pt_graphics_engine;
    renderer = pt_renderer;

    // init properties
    pxl_offset_rect_focused[0] = 0;
    pxl_offset_rect_focused[1] = 0;

    pursuit_threshold[0] = 0;
    pursuit_threshold[1] = 0;

    this->pursuit_speed = pursuit_speed / NOMINAL_FPS;

    if (SDL_GetRendererOutputSize(renderer, &rect.w, &rect.h) != 0) {
        cout << SDL_GetError() << endl;
        rect.w = 640;
        rect.h = 480;
    }

    setZoom(z);

    // move center
    moveCenterTo(center_x, center_y);
    x = rect.x;
    y = rect.y;
}

void Camera::setZoom(float z)
{
    if (z > 0.0) zoom = z;
    else zoom = 1.0;

    rect.w = RESOLUTION[0] / zoom;
    rect.h = RESOLUTION[1] / zoom;

    SDL_RenderSetLogicalSize(renderer, rect.w, rect.h);
}


void Camera::moveTo(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

void Camera::moveCenterTo(int x, int y)
{
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}

void Camera::update()
{
    if (object_focused != nullptr) // pursuit is on
    {
        SDL_Rect* rect_focused = &object_focused->rect;

        // get center pos of rect focused
        int focus_center_x = rect_focused->x + rect_focused->w / 2;
        int focus_center_y = rect_focused->y + rect_focused->h / 2;

        // process dx and dy to move for this frame
        float dx = pursuit_speed * (focus_center_x - getXCenter() + pxl_offset_rect_focused[0]);
        float dy = pursuit_speed * (focus_center_y - getYCenter() + pxl_offset_rect_focused[1]);

        if (fabs(dx) > pursuit_threshold[0]) x += dx;
        if (fabs(dy) > pursuit_threshold[1]) y += dy;

        moveTo(x , y);
    }
}

void Camera::deleteObject(GameObject *object)
{
    if (object == object_focused)
        object_focused = nullptr;
}


bool Camera::isCurrentCamera(){
    if (graphics_engine->getCurrentCamera() == this)
        return true;
    else
        return false;
}

