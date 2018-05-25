#include "graphics_engine.hpp"

using namespace std;

GraphicsEngine* GraphicsEngine::m_instance = nullptr;


GraphicsEngine* GraphicsEngine::Instance()
{
    if (!m_instance)
        m_instance = new GraphicsEngine();
    return m_instance;
}


GraphicsEngine::GraphicsEngine()
{
    //cout << "Graphics engine created" << endl;
}


// cameras
void GraphicsEngine::createCamera(SDL_Renderer* pt_renderer, int x, int y, float zoom, float pursuit_speed)
{
    Camera* camera = new Camera(this, pt_renderer, x, y, zoom, pursuit_speed);
    cameras.push_back(camera);
}

void GraphicsEngine::setCurrentCamera(Camera* pt_camera)
{
    if (pt_camera != nullptr) {
        current_camera = pt_camera;
        SDL_RenderSetLogicalSize(renderer, current_camera->rect.w, current_camera->rect.h);
    }
}

void GraphicsEngine::createCameraAndSetCurrent(SDL_Renderer* pt_renderer, int x, int y, float zoom, float pursuit_speed)
{
    createCamera(pt_renderer, x, y, zoom, pursuit_speed);
    setCurrentCamera(cameras[cameras.size() - 1]);
}

void GraphicsEngine::update()
{
    current_camera->update();
}


void GraphicsEngine::drawObjectsLayers(vector<ObjectsLayer*> *objects_layers)
{
    SDL_RenderClear(renderer);
    for (int i = int(objects_layers->size() - 1); i>=0; i--) // for each SpritesLayer
    {
        ObjectsLayer* objects_layer = (*objects_layers)[i];
        for (int j=0; j<objects_layer->size();j++){ // for each Sprite in sprite_layer
            Sprite* pt_sprite = objects_layer->objects[j];

            SDL_Rect dst_rect;
            float parallax_coefficient = objects_layer->parallax_coefficient;

            dst_rect.x = parallax_coefficient * (pt_sprite->getXCenter() - current_camera->getXCenter()) + current_camera->rect.w / 2 - pt_sprite->rect.w / 2;
            dst_rect.y = parallax_coefficient * (pt_sprite->getYCenter() - current_camera->getYCenter()) + current_camera->rect.h / 2 - pt_sprite->rect.h / 2;
            dst_rect.w = pt_sprite->rect.w;
            dst_rect.h = pt_sprite->rect.h;

            if (SDL_RenderCopyEx(renderer, pt_sprite->texture, NULL, &dst_rect, -pt_sprite->angle, &pt_sprite->center, pt_sprite->flip) != 0)
                cout << SDL_GetError() << endl;
        }
    }

    SDL_RenderPresent(renderer);
}

void GraphicsEngine::deleteObject(GameObject *object)
{
    for (unsigned int i=0; i<cameras.size(); i++)
        cameras[i]->deleteObject(object);

    texture_manager->deleteTexture(object->texture);

}


void GraphicsEngine::init()
{
    SDL_Init(SDL_INIT_VIDEO);

    // creating window
    screen = SDL_CreateWindow("Duffy and Bibine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESOLUTION[0], RESOLUTION[1], SDL_WINDOW_RESIZABLE);// | SDL_WINDOW_OPENGL);
    if ( screen == NULL)
        cout << SDL_GetError() << endl;

    // creating renderer
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);// SDL_RENDERER_SOFTWARE);
    if ( renderer == NULL)
        cout << SDL_GetError() << endl;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    if (SDL_RenderSetLogicalSize(renderer, RESOLUTION[0], RESOLUTION[1]) == -1 )
        cout << SDL_GetError() << endl;


    // background color
    // Select the color for drawing. It is set to blue/gray here.
    SDL_SetRenderDrawColor(renderer, 20, 50, 80, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(renderer);
    // Up until now everything was drawn behind the scenes.
    SDL_RenderPresent(renderer);


    texture_manager = TextureManager::Instance();
    texture_manager->init(renderer);
}

void GraphicsEngine::quit()
{
    for (unsigned int i=0; i<cameras.size(); i++)
        delete cameras[i];

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
}

