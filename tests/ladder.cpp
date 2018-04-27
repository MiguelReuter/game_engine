#include "ladder.hpp"

Ladder::Ladder(GameEngine* pt_game_engine, int x, int y, int w, int h) : StaticGameObject(x, y, w, h, 0, 10, true, true, true, true), Body(x, y, w, h, 10, true, true, true, true), Sprite(x, y, w, h)
{
    pt_game_engine->addObject(this, 1);

    // texture init
    SDL_Texture* ladder_element = pt_game_engine->graphics_engine->texture_manager->loadTexture("ladder_element");

    // create texture
    SDL_DestroyTexture(texture);
    SDL_RenderClear(renderer);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // change render target to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_Rect r; r.x = 0; r.w = w; r.h = w;
    for (int y=0; y<h; y+=w) { // draw several ladder element on final texture
        r.y = y;
        SDL_RenderCopy(renderer, ladder_element, NULL, &r);
    }
    // change render target to renderer
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(ladder_element);
};
