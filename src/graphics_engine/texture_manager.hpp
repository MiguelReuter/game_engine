#ifndef images_loader_hpp
#define images_loader_hpp

#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
//#include <windows.h>


using namespace std;

class TextureManager
{
public:
    static TextureManager* Instance();
    SDL_Renderer* renderer = nullptr;
    void init(SDL_Renderer* pt_renderer);

    // single texture
    SDL_Texture* loadTexture(string texture_name);
    void deleteTexture(SDL_Texture* pt_texture);

    // stack of textures (sprite sheets, tilesets...)
    vector<SDL_Texture*> loadTexturesStack(string textures_stack_name, const int w, const int h, const int dx = 0, const int dy = 0);
    //void deleteTexturesStack(...); string textures_stack_name (?)


private:
    // single texture
    map<string, SDL_Texture*> textures_loaded_map;
    map<string, int> textures_count_map;

    // stack of textures (sprite sheets, tilesets...)
    map<string, vector<SDL_Texture*>> textures_stacks_loaded_map;
    map<string, int> textures_stacks_count_map;

    TextureManager();
    TextureManager(TextureManager const&);
    TextureManager& operator=(TextureManager const&);
    static TextureManager* m_instance;
};


#endif /* texture_manager_hpp */
