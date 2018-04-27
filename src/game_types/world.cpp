#include "world.hpp"



using namespace std;

World* World::m_instance = nullptr;


World* World::Instance()
{
    if (!m_instance)
        m_instance = new World();
    return m_instance;
}


World::World()
{
    createSpritesLayer(0, 1.0);
    createGameObjectsLayer(0, 1.0);
    //cout << "World created" << endl;
}


void World::addObjectLayer(ObjectsLayer *pt_objects_layer)
{
    // adding created game_objects_layer to World::objects_layers, with specific position
    for(int i=0; i<objects_layers.size(); i++)
    {
        if (pt_objects_layer->getZPos() <= objects_layers[i]->getZPos()) {
            objects_layers.insert(objects_layers.begin() + i, pt_objects_layer);
            return;
        }
    }
    objects_layers.push_back(pt_objects_layer);
}


void World::createGameObjectsLayer(int z_pos, float parallax_coefficient)
{
    ObjectsLayer* game_objects_layer = new ObjectsLayer(z_pos, parallax_coefficient);
    game_objects_layers.push_back(game_objects_layer);
    
    // adding created game_objects_layer to World::objects_layers, with specific position
    addObjectLayer(game_objects_layer);
}



void World::createGameObjectsLayer(GameObject* game_object, int z_pos, float parallax_coefficient)
{
    ObjectsLayer* game_objects_layer = new ObjectsLayer(game_object, z_pos, parallax_coefficient);
    game_objects_layers.push_back(game_objects_layer);
    
    // adding created game_objects_layer to World::objects_layers, with specific position
    addObjectLayer(game_objects_layer);
}


void World::createGameObjectsLayer(vector<GameObject*> *game_objects, int z_pos, float parallax_coefficient)
{
    ObjectsLayer* game_objects_layer = new ObjectsLayer(z_pos, parallax_coefficient);
    
    for (int i=0; i<game_objects->size(); i++)
        game_objects_layer->addObject((*game_objects)[i]);
    
    game_objects_layers.push_back(game_objects_layer);
    
    // adding created game_objects_layer to World::objects_layers, with specific position
    addObjectLayer(game_objects_layer);
}

ObjectsLayer* World::getSpecificGameObjectsLayer(int z_pos)
{
    for (int i=0; i<game_objects_layers.size(); i++) {
        if (z_pos == game_objects_layers[i]->getZPos())
            return game_objects_layers[i];
    }
    return nullptr;
}

void World::addGameObjectToSpecificLayer(GameObject *object, int z_pos, float parallax_coefficient)
{
    for (int i=0; i<game_objects_layers.size(); i++) {
        if (z_pos == game_objects_layers[i]->getZPos() and parallax_coefficient == game_objects_layers[i]->parallax_coefficient) {
            game_objects_layers[i]->addObject(object);
            return;
        }
    }
    createGameObjectsLayer(object, z_pos, parallax_coefficient);
    return;
}

// -----------------------

void World::createSpritesLayer(int z_pos, float parallax_coefficient)
{
    ObjectsLayer* sprites_layer = new ObjectsLayer(z_pos, parallax_coefficient);
    sprites_layers.push_back(sprites_layer);
    
    // adding created sprites_layer to World::objects_layers, with specific position
    addObjectLayer(sprites_layer);
}


void World::createSpritesLayer(Sprite* sprite, int z_pos, float parallax_coefficient)
{
    ObjectsLayer* sprites_layer = new ObjectsLayer(sprite, z_pos, parallax_coefficient);
    sprites_layers.push_back(sprites_layer);
    
    // adding created sprites_layer to World::objects_layers, with specific position
    addObjectLayer(sprites_layer);
}


void World::createSpritesLayer(vector<Sprite*> *sprites, int z_pos, float parallax_coefficient)
{
    ObjectsLayer* sprites_layer = new ObjectsLayer(*sprites, z_pos, parallax_coefficient);
    sprites_layers.push_back(sprites_layer);
    
    // adding created sprites_layer to World::objects_layers, with specific position
    addObjectLayer(sprites_layer);
}

void World::addSpriteToSpecificLayer(Sprite *object, int z_pos, float parallax_coefficient)
{
    for (int i=0; i<sprites_layers.size(); i++) {
        if (z_pos == sprites_layers[i]->getZPos() and parallax_coefficient == sprites_layers[i]->parallax_coefficient) {
            sprites_layers[i]->addObject(object);
            return;
        }
    }
    createSpritesLayer(object, z_pos, parallax_coefficient);
    return;
}

ObjectsLayer* World::getSpecificSpritesLayer(int z_pos)
{
    for (int i=0; i<sprites_layers.size(); i++) {
        if (z_pos == sprites_layers[i]->getZPos())
            return sprites_layers[i];
    }
    return nullptr;
}

void World::init()
{}

void World::update() {
    for (int i=0; i<objects_layers.size(); i++) {
        int objects_nb = int(objects_layers[i]->size());
        for (int j=0; j<objects_nb; j++)
            objects_layers[i]->objects[j]->update();
    }
}

void World::deleteObjectFromLayers(GameObject *object, vector<ObjectsLayer *> layers)
{
    for (int i=0; i<layers.size(); i++) {
        ObjectsLayer* objects_layer = layers[i];
        for (int j=0; j<objects_layer->size(); j++) {
            vector<Sprite*> *pt_objects = &objects_layer->objects;
            Sprite* cast_object = static_cast<Sprite*>(object);
            
            if (cast_object == (*pt_objects)[j])
                pt_objects->erase(pt_objects->begin() + j);
        }
    }
}

void World::deleteObject(GameObject *object)
{
    // delete object from game_objects_layers, sprites_layers and objects_layers
    deleteObjectFromLayers(object, game_objects_layers);
    deleteObjectFromLayers(object, sprites_layers);
    deleteObjectFromLayers(object, objects_layers);
}


void World::quit()
{
    // delete all objects
    for (int i=0; i<objects_layers.size(); i++) {
        for (int j=0; j<objects_layers[i]->objects.size(); j++)
            delete objects_layers[i]->objects[j];
    }
    
    // clear layers of objects
    objects_layers.clear();
    sprites_layers.clear();
    game_objects_layers.clear();
}




