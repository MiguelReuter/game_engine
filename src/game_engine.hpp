#ifndef GameEngine_hpp
#define GameEngine_hpp

#include <iostream>
#include <stdio.h>

#include "constants.h"

#include "events_engine/events.h"
#include "graphics_engine/graphics.h"
#include "physics_engine/physics.h"
#include "sound_engine/sound.h"
#include "game_types/world.hpp"
#include "game_types/objects_layer.hpp"
#include "game_types/game_types.h"


class GameEngine
{
public:
    static GameEngine* Instance();
    EventsEngine* events_engine;
    GraphicsEngine* graphics_engine;
    PhysicsEngine* physics_engine;
    SoundEngine* sound_engine;
    World* world;
    void init();
    void update();

    // add differents objects
    void addObject(CircleDynamicGameObject* object, int z_pos=0, float parallax_coefficient=1.0);
    void addObject(DynamicGameObject* object, int z_pos=0, float parallax_coefficient=1.0);
    void addObject(StaticGameObject* object, int z_pos=0, float parallax_coefficient=1.0);
    void addObject(Sprite* object, int z_pos=0, float parallax_coefficient=1.0);
    void addObject(GameObject* object, int z_pos=0, float parallax_coefficient=1.0);

    void deleteObject(GameObject* object);

    void quit();


private:
    GameEngine();
    GameEngine(GameEngine const&);
    GameEngine& operator=(GameEngine const&);
    static GameEngine* m_instance;
};

#endif /* GameEngine_hpp */
