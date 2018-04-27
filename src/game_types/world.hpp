#ifndef World_hpp
#define World_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

#include "game_types.h"
#include "objects_layer.hpp"

using namespace std;


class GameObject;

class World
{
public:
    static World* Instance();

    // game_objects_layers
    void createGameObjectsLayer(int z_pos, float parallax_coefficient);
    void createGameObjectsLayer(GameObject* game_object, int z_pos, float parallax_coefficient);
    void createGameObjectsLayer(vector<GameObject*> *game_objects, int z_pos, float parallax_coefficient);
    ObjectsLayer* getSpecificGameObjectsLayer(int z_pos);
    void addGameObjectToSpecificLayer(GameObject* object, int z_pos=0, float parallax_coefficient=1.0);

    // sprites_layers
    void createSpritesLayer(int z_pos, float parallax_coefficient);
    void createSpritesLayer(Sprite* sprite, int z_pos, float parallax_coefficient);
    void createSpritesLayer(vector<Sprite*> *sprites, int z_pos, float parallax_coefficient);
    ObjectsLayer* getSpecificSpritesLayer(int z_pos);
    void addSpriteToSpecificLayer(Sprite* object, int z_pos=0, float parallax_coefficient=1.0);

    // delete specific object from layer of objects_layers, game_objects_layers and/or sprites_layers
    void deleteObject(GameObject* object);

    vector<ObjectsLayer*>* getObjectsLayers(){return &objects_layers;};

    void update();
    void init();
    void quit();


private:
    World();
    World(World const&);
    World& operator=(World const&);
    static World* m_instance;

    vector <ObjectsLayer*> game_objects_layers;  // ObjectsLayer container NOT sorted by z_pos values
    vector <ObjectsLayer*> sprites_layers; // ObjectsLayer container NOT sorted by z_pos values
    vector <ObjectsLayer*> objects_layers; // ObjectsLayer container : includes game_objects_layers and sprites_layers, SORTED by z_pos values

    void addObjectLayer(ObjectsLayer* pt_objects_layer);
    void deleteObjectFromLayers(GameObject* object, vector<ObjectsLayer*> layers);
};



#endif /* World_hpp */
