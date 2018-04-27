#include "rigged_test.hpp"

Riggedtest::Riggedtest(GameEngine* pt_game_engine, int x, int y) : GameObject(x, y, 32, 32, 0, 10, false, false, false, false), Body(x, y, 32, 32, 10, false, false, false, false), RiggedSprite(x, y, 32, 32), Sprite(x, y, 32, 32)
{
    SDL_Point origin; origin.x = 0; origin.y = 0;
    map<string, Bone*> m;
    
    Bone* arm = new Bone(0, 0, 0, 100);
    Bone* arm2 = new Bone(50, 0, 0, 50, arm);
    Bone* arm3 = new Bone(0, 0, 0, 50, arm2);
    m["arm"] = arm;
    m["arm2"] = arm2;
    m["arm3"] = arm3;
    armature = new Armature(origin, m);

    pt_game_engine->addObject(this);
}

void Riggedtest::init(SDL_Renderer *pt_renderer)
{
    GameObject::init(pt_renderer);
    RiggedSprite::init(pt_renderer);
}


void Riggedtest::update()
{
    i += 1;

    SDL_Point point;
    point.x = i % 60;
    point.y = 0;
    
    armature->setBoneGlobalAngle("arm", i/2.0);
    armature->setBoneLocalAngle("arm3", -i/4.0);
    
    armature->setBoneLocalPosition("arm2", point);
    armature->setBoneLocalAngle("arm2", i/2);
    GameObject::update();
    RiggedSprite::update();
}
