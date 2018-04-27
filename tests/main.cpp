#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


#include "../src/game_engine.hpp"

#include "box.hpp"
#include "character.hpp"
#include "ladder.hpp"
#include "rigged_test.hpp"

using namespace std;

void loadMap(GameEngine* game_engine)
{
    string map_representation[] =
    {"                    ",
        "                    ",
        "                    ",
        "4                   ",
        "            444     ",
        "  44                ",
        "  4               85",
        "7                533",
        "c4447477747687448636",
        "a9c9a999a9acaccc3363"};

    int img_src_tile_size = 196, tile_size = 32;
    int line_nb = 10;
    int col_nb = 20;

    // load tileset
    SDL_Rect r;
    r.w = img_src_tile_size;
    r.h = img_src_tile_size;
    vector<SDL_Texture*> tiles = game_engine->graphics_engine->texture_manager->loadTexturesStack("map_tileset", 196, 196);

    // creating map
    char char_i_j;
    string hex_str;

    for (int i=0;i<line_nb;i++) {
        for (int j=0;j<col_nb;j++) {
            char_i_j = map_representation[i][j];
            if ( char_i_j != ' ') {
                hex_str = "0x";
                hex_str.push_back(char_i_j);

                StaticGameObject* object = new StaticGameObject(j * tile_size, i * tile_size, 32, 32);
                game_engine->addObject(object);

                object->texture = tiles[int(stoul(hex_str, nullptr, 16))];
            }
        }
    }
}


int main(int argc, char * argv[]) {
    GameEngine* game_engine = GameEngine::Instance();
    game_engine->init();

    // ---------- ADDING OBJECTS ----------
    // rigged test
    //Riggedtest* rigged_test = new Riggedtest(game_engine, 288, 224);

    // character
    Character* character = new Character(game_engine, 200, 50, 61, 64);

    // map
    loadMap(game_engine);

    // some boxes
    new Box(game_engine, 340, 224);
    new Box(game_engine, 330, 190);
    // new Box(game_engine, 255, 150);


    // a platform
    StaticGameObject* platform = new StaticGameObject(100, 128, 64, 32, 0, 10, true, false, true, true);
    game_engine->addObject(platform, 1);

    // a ladder
    Ladder* ladder = new Ladder(game_engine, 352, 128, 32, 128);


    // one way wall
    /*
    StaticGameObject* one_way_wall = new StaticGameObject(100, 200, 32, 64, 0, 10, true, false, false, false);
    game_engine->addObject(one_way_wall, 0);*/


    SDL_Renderer* renderer = game_engine->graphics_engine->renderer;
    // background
    Sprite* pt_background_sprite = new Sprite(0, 0, 640, 480);
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 200, 200, 200));
    SDL_Texture *background_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    game_engine->addObject(pt_background_sprite, 100, 1.0);
    pt_background_sprite->texture = background_tex;

    // red square
    /*
    Sprite* pt_red_sprite = new Sprite(500, 240, 32, 32);
    SDL_Surface *square_surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(square_surface, NULL, SDL_MapRGB(square_surface->format, 200, 0, 0));
    SDL_Texture *red_texture = SDL_CreateTextureFromSurface(renderer, square_surface);
    SDL_FreeSurface(square_surface);
    pt_red_sprite->texture = red_texture;
    game_engine->addObject(pt_red_sprite, -1, 1.2);
    */


    // focus camera
    game_engine->graphics_engine->getCurrentCamera()->setFocusOn(character);
    game_engine->graphics_engine->getCurrentCamera()->setZoom(2);

    int frames_nb = 0;
    int t2, t1, T2, T1;

    T1 = SDL_GetTicks();

    while (!game_engine->events_engine->in.quit) {
        t1 = SDL_GetTicks();

        // ---------- EVENTS ----------
        // character
        // run at left
        if (game_engine->events_engine->in.key[SDL_SCANCODE_LEFT] == KEY_PRESSED)
            character->move(LEFT_MOVE);
        // run at right
        else if (game_engine->events_engine->in.key[SDL_SCANCODE_RIGHT] == KEY_PRESSED)
            character->move(RIGHT_MOVE);
        else
            character->move(NO_MOVE);


        // jump
        if (game_engine->events_engine->in.key[SDL_SCANCODE_SPACE] == KEY_JUST_PRESSED)
            character->jump();

        // hit
        if (game_engine->events_engine->in.key[SDL_GetScancodeFromKey(SDLK_a)] == KEY_JUST_PRESSED)
            character->hit();

        // crouch or go down on a ladder
        if (game_engine->events_engine->in.key[SDL_SCANCODE_DOWN] == KEY_JUST_PRESSED or game_engine->events_engine->in.key[SDL_SCANCODE_DOWN] == KEY_PRESSED)
        {
            character->climb(false);
            character->crouch();
        }

        // stand up
        if (game_engine->events_engine->in.key[SDL_SCANCODE_DOWN] == KEY_RELEASED)
            character->stand_up();

        // climb
        if (game_engine->events_engine->in.key[SDL_SCANCODE_UP] == KEY_PRESSED)
            character->climb(true);


        // platform
        int dx = 0, dy = 0;
        if (game_engine->events_engine->in.key[SDL_GetScancodeFromKey(SDLK_q)] == KEY_PRESSED)
            dx -= 2;
        if (game_engine->events_engine->in.key[SDL_SCANCODE_D] == KEY_PRESSED)
            dx += 2;
        if (game_engine->events_engine->in.key[SDL_SCANCODE_W] == KEY_PRESSED)
            dy -= 2;
        if (game_engine->events_engine->in.key[SDL_SCANCODE_S] == KEY_PRESSED)
            dy += 2;
        platform->move(dx, dy);


        // adding projectiles
        /*
        if (game_engine->events_engine->in.key[SDL_GetScancodeFromKey(SDLK_a)] == KEY_JUST_PRESSED)
        {
            DynamicGameObject* projectile = new DynamicGameObject(100, 50, 16, 16, 3, 15, 0.8, 5);
            projectile->setGhostForPrimaryDynamicBody();
            character->addObject(projectile, 35, 0, 0, -10);


            /*projectile->dxy[0] = 10; projectile->dxy[1] = 0; //-10;
            game_engine->addObject(projectile);
            SDL_DestroyTexture(projectile->texture);
            projectile->texture = game_engine->graphics_engine->texture_manager->loadTexture("ball");
        }*/

        // ---------- GAME ENGINE UPDATE ----------
        game_engine->update();

        // time
        t2 = SDL_GetTicks();

        if (t2 - t1 < 1000.0 * NOMINAL_DT){
            //float time_ratio = (t2 - t1) / 16.6;
            //if (time_ratio > 0.1) cout << 100 * time_ratio << "%" << endl;
            SDL_Delay(1000.0 * NOMINAL_DT - (t2 - t1));
        }
        frames_nb += 1;
    }
    T2 = SDL_GetTicks();

    cout << float(frames_nb * 1000) / (T2 - T1) << " mean FPS" << endl;

    game_engine->quit();
    SDL_Quit();
    return 0;
}






