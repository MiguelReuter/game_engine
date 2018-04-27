#include "rigged_sprite.hpp"

RiggedSprite::RiggedSprite(int x, int y, int w, int h) : Sprite(x, y, w, h)
{
    delta_origin.x = 0;
    delta_origin.y = 0;
};


void RiggedSprite::init(SDL_Renderer *pt_renderer)
{
    Sprite::init(pt_renderer);
    armature->init(pt_renderer);
}

void RiggedSprite::update()
{
    Sprite::update();

    SDL_Point top_left;
    SDL_Point bottom_right;
    armature->getExtremePointsReachable(&top_left, &bottom_right);

    delta_origin = top_left;
    rect.x += delta_origin.x;
    rect.y += delta_origin.y;
    rect.w = bottom_right.x - top_left.x;
    rect.h = bottom_right.y - top_left.y;


    // create texture
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // change render target to texture
    if (SDL_SetRenderTarget(renderer, texture) != 0)
        cout << SDL_GetError() << endl;
    SDL_RenderClear(renderer);

    // For each bone
    for(map<string, Bone*>::iterator it = armature->bones.begin(); it != armature->bones.end(); ++it) {
        Bone* bone = it->second;
        double bone_angle = armature->getBoneAngle(it->first);
        SDL_Point bone_pos = armature->getBonePosition(it->first);
        SDL_Point* bone_sprite_center = &bone->sprite->center;

        double angle = bone_angle + bone->sprite->angle;

        // coordinates of top left corner's sprite (before rotation) in bone referential
        float x_s_wor_b = -bone_sprite_center->x * cos(bone_angle * PI / 180) -bone_sprite_center->y * sin(bone_angle * PI / 180);
        float y_s_wor_b = bone_sprite_center->x * sin(bone_angle * PI / 180) -bone_sprite_center->y * cos(bone_angle * PI / 180);

        //+ center pos in bone ref
        x_s_wor_b += bone_sprite_center->x + bone->sprite->rect.x;
        y_s_wor_b += bone_sprite_center->y + bone->sprite->rect.y;


        SDL_Rect bone_rect;
        bone_rect.x = bone_pos.x + x_s_wor_b * cos(bone_angle * PI / 180) + y_s_wor_b * sin(bone_angle * PI / 180) - top_left.x;
        bone_rect.y = bone_pos.y - x_s_wor_b * sin(bone_angle * PI / 180) + y_s_wor_b * cos(bone_angle * PI / 180) - top_left.y;

        bone_rect.w = bone->sprite->rect.w;
        bone_rect.h = bone->sprite->rect.h;

        if (SDL_RenderCopyEx(renderer, bone->sprite->texture, NULL, &bone_rect, -angle, bone_sprite_center, bone->sprite->flip) != 0)
            cout << SDL_GetError() << endl;
    }

    // change render target to renderer
    if (SDL_SetRenderTarget(renderer, NULL) != 0)
        cout << SDL_GetError() << endl;
}

RiggedSprite::~RiggedSprite()
{
    delete armature;
}
