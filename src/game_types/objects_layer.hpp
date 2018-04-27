#ifndef objects_layer_hpp
#define objects_layer_hpp

#include <stdio.h>
#include <vector>

#include "../graphics_engine/sprite.hpp"

using namespace std;

class ObjectsLayer
{
public:
    float parallax_coefficient;
    int getZPos() { return z_pos;};
    long virtual size(){return objects.size();};
    vector<Sprite*> objects;

    void deleteObject(Sprite* object);
    void addObject(Sprite* object){objects.push_back(object);};
    void addObjects(vector<Sprite*> objects_list){objects.insert(objects.end(), objects_list.begin(), objects_list.end());};

    ObjectsLayer(int z_pos=0, float parallax_coefficient = 1.0);
    ObjectsLayer(Sprite* object, int z_pos=0, float parallax_coefficient = 1.0);
    ObjectsLayer(vector<Sprite*> objects_list, int z_pos=0, float parallax_coefficient = 1.0);
    ~ObjectsLayer();

protected:
    int z_pos; // screen < 0 , background > 0
};





#endif /* objects_layer_hpp */
