#include "bone.hpp"

Bone::Bone(int global_x, int global_y, float f_global_angle, float f_length)
{
    length = f_length;
    
    // local
    setLocalAngle(0);
    setLocalHeadPosition(0, 0);

    // global
    setGlobalAngle(f_global_angle);
    setGlobalHeadPosition(global_x, global_y);
}

Bone::Bone(int local_x, int local_y, float f_local_angle, float f_length, Bone* b_parent)
{
    parent = b_parent;
    parent->children.push_back(this);
    length = f_length;
    
    // local
    setLocalAngle(f_local_angle);
    setLocalHeadPosition(local_x, local_y);
    
    // global
    setGlobalAngle(0);
    setGlobalHeadPosition(0, 0);
}

void Bone::setLocalHeadPosition(int x, int y)
{
    // update local tail's position
    local_tail.x += x - local_head.x;
    local_tail.y += y - local_head.y;
    
    // update local head's position
    local_head.x = x;
    local_head.y = y;
}

void Bone::setLocalAngle(float f_angle)
{
    // update local angle
    local_angle = f_angle;
    
    // update local tail's position
    local_tail.x = local_head.x + length * cos(local_angle * PI / 180);
    local_tail.y = local_head.y - length * sin(local_angle * PI / 180);
}


void Bone::setGlobalHeadPosition(int x, int y)
{
    // update global tail's position
    global_tail.x += x - global_head.x;
    global_tail.y += y - global_head.y;
    
    // update global head's position
    global_head.x = x;
    global_head.y = y;
}


void Bone::setGlobalAngle(float f_angle)
{
    // update global angle
    global_angle = f_angle;
    
    // update global tail's position
    global_tail.x = global_head.x + length * cos(global_angle * PI / 180);
    global_tail.y = global_head.y - length * sin(global_angle * PI / 180);
}


SDL_Point Bone::getAbsoluteHeadPosition()
{
    if (parent == nullptr)
        return global_head;
    else {
        SDL_Point absolute_head_pos;
        SDL_Point absolute_parent_tail_pos = parent->getAbsoluteTailPosition();
        double abs_angle = getLocalAngle();
        
        absolute_head_pos.x = absolute_parent_tail_pos.x + local_head.x * cos(abs_angle * PI / 180) + local_head.y * sin(abs_angle * PI / 180);
        absolute_head_pos.y = absolute_parent_tail_pos.y - local_head.x * sin(abs_angle * PI / 180) + local_head.y * cos(abs_angle * PI / 180);

        return absolute_head_pos;
    }
}


SDL_Point Bone::getAbsoluteTailPosition()
{
    if (parent == nullptr)
        return global_tail;
    else {
        SDL_Point absolute_head_pos = getAbsoluteHeadPosition();
        SDL_Point absolute_tail_pos;
        
        absolute_tail_pos.x = absolute_head_pos.x + length * cos(getAbsoluteAngle() * PI / 180);
        absolute_tail_pos.y = absolute_head_pos.y - length * sin(getAbsoluteAngle() * PI / 180);

        return absolute_tail_pos;
    }
}


float Bone::getAbsoluteAngle()
{
    if (parent == nullptr)
        return global_angle;
    else
        return parent->getAbsoluteAngle() + local_angle;
}

Bone::~Bone()
{
    delete sprite;
}



