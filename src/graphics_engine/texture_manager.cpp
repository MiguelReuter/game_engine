#include "texture_manager.hpp"

using namespace std;


TextureManager* TextureManager::m_instance = nullptr;


TextureManager* TextureManager::Instance()
{
    if (!m_instance)
        m_instance = new TextureManager();
    return m_instance;
}


TextureManager::TextureManager()
{}


void TextureManager::init(SDL_Renderer* pt_renderer){renderer = pt_renderer;}



SDL_Texture* TextureManager::loadTexture(string texture_name)
{
    map<string, const char*> IMAGES_PATH_MAP = {
        {"ball", "data/circle.png"},
        {"tileset", "data/tileset.png"},
        {"box", "data/box.png"},
        {"ladder_element", "data/ladder_element.png"},
        {"character_sprite_sheet", "data/lucas_sprite_sheet.png"}
    };


    for(map<string, SDL_Texture*>::iterator it = textures_loaded_map.begin(); it != textures_loaded_map.end(); ++it) {
        if (texture_name == it->first) {
            textures_count_map[texture_name] += 1;
            return textures_loaded_map[texture_name];
        }
    }


    for(map<string, const char*>::iterator it = IMAGES_PATH_MAP.begin(); it != IMAGES_PATH_MAP.end(); ++it) {
        if (texture_name == it->first) {
            SDL_Surface* pt_surface = IMG_Load(IMAGES_PATH_MAP[texture_name]);
            if (pt_surface == nullptr)
                cout << SDL_GetError() << endl;
            else {
                SDL_Texture* pt_texture = SDL_CreateTextureFromSurface(renderer, pt_surface);
                textures_loaded_map[texture_name] = pt_texture;
                SDL_FreeSurface(pt_surface);

                textures_count_map[texture_name] = 1;
                return pt_texture;
            }
        }
    }

    cout << texture_name << " not found in IMAGES_PATH_MAP" << endl;
    return nullptr;
}


void TextureManager::deleteTexture(SDL_Texture *pt_texture)
{
    for(map<string, SDL_Texture*>::iterator it = textures_loaded_map.begin(); it != textures_loaded_map.end(); ++it) {
        if (pt_texture == it->second) {
            string texture_name = it->first;
            textures_count_map[texture_name] -= 1;
            if (textures_count_map[texture_name] == 0) {
                SDL_DestroyTexture(pt_texture);
                textures_loaded_map.erase(texture_name);
                textures_count_map.erase(texture_name);
            }
            break;
        }
    }
    SDL_DestroyTexture(pt_texture);
}

vector<SDL_Texture*> TextureManager::loadTexturesStack(string textures_stack_name, const int w, const int h, const int dx, const int dy)
{
    map<string, const char*> IMAGES_PATH_MAP = {
    {"character_sprite_sheet", "data/lucas_sprite_sheet.png"},
    {"duffy_sprite_sheet", "data/duffy_sprite_sheet_3.png"},
    {"map_tileset", "data/tileset.png"}
    };

    // if textures_stack_name loaded yet
    for(map<string, vector<SDL_Texture*>>::iterator it = textures_stacks_loaded_map.begin(); it != textures_stacks_loaded_map.end(); ++it) {
        if (textures_stack_name == it->first) {
            textures_stacks_count_map[textures_stack_name] += 1;
            return textures_stacks_loaded_map[textures_stack_name];
        }
    }


    for(map<string, const char*>::iterator it = IMAGES_PATH_MAP.begin(); it != IMAGES_PATH_MAP.end(); ++it) {
        if (textures_stack_name == it->first) {

            SDL_Surface* pt_surface_stack = IMG_Load(IMAGES_PATH_MAP[textures_stack_name]);

            if (pt_surface_stack == nullptr)
                cout << SDL_GetError() << endl;
            else {
                SDL_Texture* pt_texture_stack = SDL_CreateTextureFromSurface(renderer, pt_surface_stack);
                vector<SDL_Texture*> textures_stack;

                int col_nb = pt_surface_stack->w / w;
                int lin_nb = pt_surface_stack->h / h;

                SDL_Rect rect; rect.w = w; rect.h = h;

                for (int j=0; j<lin_nb; j++) {
                    rect.y = j * h + dy;
                    for (int i=0; i<col_nb; i++) {
                        rect.x = i * w + dx;

                        // create texture
                        SDL_Texture* pt_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
                        SDL_SetTextureBlendMode(pt_texture, SDL_BLENDMODE_BLEND);

                        // change render target to texture
                        SDL_SetRenderTarget(renderer, pt_texture);

                        SDL_RenderCopy(renderer, pt_texture_stack, &rect, NULL);
                        // change render target to renderer
                        SDL_SetRenderTarget(renderer, NULL);

                        textures_stack.push_back(pt_texture);
                    }
                }

                SDL_FreeSurface(pt_surface_stack);
                SDL_DestroyTexture(pt_texture_stack);
                textures_stacks_loaded_map[textures_stack_name] = textures_stack;
                textures_stacks_count_map[textures_stack_name] = 1;
                return textures_stack;
            }
        }
    }

    cout << textures_stack_name << " not found in IMAGES_PATH_MAP" << endl;
    return {nullptr};
}

