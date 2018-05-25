#include "game_engine.hpp"

using namespace std;

GameEngine* GameEngine::m_instance = nullptr;


GameEngine* GameEngine::Instance()
{
    if (!m_instance)
        m_instance = new GameEngine;
    return m_instance;
}


GameEngine::GameEngine()
{
    //cout << "Game engine created" << endl;
    events_engine = EventsEngine::Instance();
    graphics_engine = GraphicsEngine::Instance();
    physics_engine = PhysicsEngine::Instance();
    sound_engine = SoundEngine::Instance();
    world = World::Instance();
}

void GameEngine::update(void)
{
    // delete objects to delete
    for (uint i=0; i<events_engine->in.game_objects_to_delete.size(); i++)
        deleteObject(events_engine->in.game_objects_to_delete[i]);

    // add objects to add
    for (uint i=0; i<events_engine->in.game_objects_to_add.size(); i++) {
        GameObject* object = events_engine->in.game_objects_to_add[i];

        // CircleDynamicGameObject
        if (dynamic_cast<CircleDynamicGameObject*>(object) != nullptr)
            addObject(dynamic_cast<CircleDynamicGameObject*>(object));

        // DynamicGameObject
        else if (dynamic_cast<DynamicGameObject*>(object) != nullptr)
            addObject(dynamic_cast<DynamicGameObject*>(object));

        // StaticGameObject
        else if (dynamic_cast<StaticGameObject*>(object) != nullptr)
            addObject(dynamic_cast<StaticGameObject*>(object));

        else
            addObject(object);
    }

    events_engine->in.game_objects_to_delete.clear();
    events_engine->in.game_objects_to_add.clear();

    // ---------- EVENTS ----------
    events_engine->update();

    // ---------- PHYSICS ----------
    physics_engine->update();

    // ---------- WORLD ----------
    world->update();

    // ---------- GRAPHICS ----------
    graphics_engine->update();
    graphics_engine->drawObjectsLayers(world->getObjectsLayers());
}

void GameEngine::init()
{
    events_engine->init();

    // Graphics init
    graphics_engine->init();
    // camera
    graphics_engine->createCameraAndSetCurrent(graphics_engine->renderer, 320, 240, 0.8, 10);


    world->init();
    // test focus
    //graphics_engine->getCurrentCamera()->setFocusOn(...); //GameObject*

}

// ---------------- ADD OBJECTS ----------------

void GameEngine::addObject(CircleDynamicGameObject *object, int z_pos, float parallax_coefficient)
{
    physics_engine->addCircleDynamicBody(object);
    world->addGameObjectToSpecificLayer(object, z_pos, parallax_coefficient);
    object->init(graphics_engine->renderer);
}

void GameEngine::addObject(DynamicGameObject *object, int z_pos, float parallax_coefficient)
{
    physics_engine->addDynamicBody(object);
    world->addGameObjectToSpecificLayer(object, z_pos, parallax_coefficient);
    object->init(graphics_engine->renderer);
}


void GameEngine::addObject(Sprite *object, int z_pos, float parallax_coefficient)
{
    world->addSpriteToSpecificLayer(object, z_pos, parallax_coefficient);
    object->init(graphics_engine->renderer);
}


void GameEngine::addObject(StaticGameObject *object, int z_pos, float parallax_coefficient)
{
    physics_engine->addStaticBody(object);
    world->addGameObjectToSpecificLayer(object, z_pos, parallax_coefficient);
    object->init(graphics_engine->renderer);
}

void GameEngine::addObject(GameObject *object, int z_pos, float parallax_coefficient)
{
    // Dynamic body
    DynamicBody* dynamic_object = dynamic_cast<DynamicBody*>(object);
    if ( dynamic_object != nullptr )
        physics_engine->addDynamicBody(dynamic_object);

    // Static body
    StaticBody* static_object = dynamic_cast<StaticBody*>(object);
    if ( static_object != nullptr )
        physics_engine->addStaticBody(static_object);

    world->addSpriteToSpecificLayer(object, z_pos, parallax_coefficient);
    object->init(graphics_engine->renderer);

}

void GameEngine::deleteObject(GameObject *object)
{
    physics_engine->deleteObject(object);
    graphics_engine->deleteObject(object);
    world->deleteObject(object);
    delete object;
}


void GameEngine::quit()
{
    graphics_engine->quit();
    physics_engine->quit();
    world->quit();
}


