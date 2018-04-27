#include "armature.hpp"

Armature::Armature(SDL_Point p_origin, map<string, Bone*> m_bones)
{
    origin = p_origin;
    bones = m_bones;
}

Armature::Armature(SDL_Point p_origin)
{
    origin = p_origin;
}

SDL_Point Armature::getBonePosition(string bone_name)
{
    Bone* bone = bones[bone_name];
    return bone->getAbsoluteHeadPosition();
}

float Armature::getBoneAngle(string bone_name)
{
    Bone* bone = bones[bone_name];
    return bone->getAbsoluteAngle();
}

float Armature::getBoneLength(string bone_name)
{
    Bone* bone = bones[bone_name];
    return bone->length;
}


void Armature::setBoneLocalPosition(string bone_name, SDL_Point point)
{
    Bone* bone = bones[bone_name];
    bone->setLocalHeadPosition(point);
}

void Armature::setBoneLocalAngle(string bone_name, float f_angle)
{
    Bone* bone = bones[bone_name];
    bone->setLocalAngle(f_angle);
}

void Armature::setBoneGlobalPosition(string bone_name, SDL_Point point)
{
    Bone* bone = bones[bone_name];
    bone->setGlobalHeadPosition(point);
}

void Armature::setBoneGlobalAngle(string bone_name, float f_angle)
{
    Bone* bone = bones[bone_name];
    bone->setGlobalAngle(f_angle);
}


void Armature::getExtremePointsReachable(SDL_Point *top_left, SDL_Point *bottom_right)
{
    top_left->x = 0; top_left->y = 0;
    bottom_right->x = 0; bottom_right->y = 0;
    
    // For each bone
    for(map<string, Bone*>::iterator it = bones.begin(); it != bones.end(); ++it) {
        Bone* bone = it->second;
        SDL_Point bone_pos = getBonePosition(it->first);
        float bone_angle = getBoneAngle(it->first);
        
        // process coordinates of the 4 corners of sprite's bone in bone referential
        // the sprite has the following transformations : translation + rotation around its center
        Sprite* bone_sprite = bone->sprite;
        SDL_Point sprite_corners[4];
        // translation
        sprite_corners[0].x = -bone_sprite->center.x; sprite_corners[0].y = -bone_sprite->center.y;
        sprite_corners[1].x = bone_sprite->rect.w - bone_sprite->center.x; sprite_corners[1].y = -bone_sprite->center.y;
        sprite_corners[2].x = bone_sprite->rect.w - bone_sprite->center.x; sprite_corners[2].y = bone_sprite->rect.h - bone_sprite->center.y;
        sprite_corners[3].x = -bone_sprite->center.x; sprite_corners[3].y = bone_sprite->rect.h - bone_sprite->center.y;
        
        // rotation around center
        for (int i=0; i<4; i++) {
            float x_corner = sprite_corners[i].x * cos(bone_sprite->angle * PI / 180) + sprite_corners[i].y * sin(bone_sprite->angle * PI / 180);
            float y_corner = -sprite_corners[i].x * sin(bone_sprite->angle * PI / 180) + sprite_corners[i].y * cos(bone_sprite->angle * PI / 180);
            
            
            // + center pos
            sprite_corners[i].x = x_corner + bone_sprite->rect.x + bone_sprite->center.x;
            sprite_corners[i].y = y_corner + bone_sprite->rect.y + bone_sprite->center.y;
        }
        
        // process coordinates of the 4 corners in armature's referential
        // the bone has the following transformation : translation + rotation around its head, ie around bone's position
        for (int i=0; i<4; i++) {
            float x_corner = sprite_corners[i].x * cos(bone_angle * PI / 180) + sprite_corners[i].y * sin(bone_angle * PI / 180) + bone_pos.x;
            float y_corner = -sprite_corners[i].x * sin(bone_angle * PI / 180) + sprite_corners[i].y * cos(bone_angle * PI / 180) + bone_pos.y;
            
            // set extreme points
            top_left->x = min(top_left->x, int(x_corner));
            top_left->y = min(top_left->y, int(y_corner));
            bottom_right->x = max(bottom_right->x, int(x_corner));
            bottom_right->y = max(bottom_right->y, int(y_corner));
        }
    }
}




void Armature::init(SDL_Renderer *pt_renderer)
{
    int h = 10;
    int i = 1;
    for(map<string, Bone*>::iterator it = bones.begin(); it != bones.end(); ++it) {
        Bone* bone = it->second;
        bone->sprite = new Sprite(-h/2, -h/2, bone->length + h, h);
        bone->sprite->center.x = 0;
        bone->sprite->center.y = 0;
        bone->sprite->renderer = pt_renderer;
        
        SDL_Surface* pt_surface = SDL_CreateRGBSurface(0, bone->length + h, h, 32, 0, 0, 0, 0);
        SDL_FillRect(pt_surface, NULL, SDL_MapRGB(pt_surface->format, 120, 100+20*i, 100 + 20*i));
        bone->sprite->texture = SDL_CreateTextureFromSurface(pt_renderer, pt_surface);
        SDL_FreeSurface(pt_surface);
        i += 1;
    }
}

Armature::~Armature()
{
    for(map<string, Bone*>::iterator it = bones.begin(); it != bones.end(); ++it)
        delete it->second;
}





