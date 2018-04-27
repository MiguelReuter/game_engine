#ifndef graphics_hpp
#define graphics_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

#include "camera.hpp"
#include "texture_manager.hpp"
#include "../game_types/objects_layer.hpp"
#include "animation/rigged_sprite.hpp"


using namespace std;

class Camera;

class GraphicsEngine
{
public:
    static GraphicsEngine* Instance();
    SDL_Renderer* renderer = nullptr;
    SDL_Window* screen = nullptr;

    // draw
    void drawObjectsLayers(vector<ObjectsLayer*> *objects_layers);

    // cameras
    void createCameraAndSetCurrent(SDL_Renderer* pt_renderer, int x = 0, int y = 0, float zoom = 1.0, float pursuit_speed = 0.5);
    void createCamera(SDL_Renderer* pt_renderer, int x = 0, int y = 0, float zoom = 1.0, float pursuit_speed = 0.5);

    void setCurrentCamera(Camera* pt_camera);
    Camera* getCurrentCamera(){return current_camera;}

    vector <Camera*> cameras;

    void deleteObject(GameObject* object);

    void update();
    void init();
    void quit();

    TextureManager* texture_manager = nullptr;

private:
    GraphicsEngine();
    GraphicsEngine(GraphicsEngine const&);
    GraphicsEngine& operator=(GraphicsEngine const&);
    static GraphicsEngine* m_instance;

    Camera* current_camera = nullptr;
};


#endif /* graphics_hpp */
