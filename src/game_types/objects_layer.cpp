#include "objects_layer.hpp"

ObjectsLayer::ObjectsLayer(int z_pos, float parallax_coefficient)
{
    this->z_pos = z_pos;
    this->parallax_coefficient = parallax_coefficient;
}

ObjectsLayer::ObjectsLayer(Sprite* object, int z_pos, float parallax_coefficient)
{
    this->z_pos = z_pos;
    this->parallax_coefficient = parallax_coefficient;
    this->addObject(object);
}

ObjectsLayer::ObjectsLayer(vector<Sprite*> objects_list, int z_pos, float parallax_coefficient)
{
    this->z_pos = z_pos;
    this->parallax_coefficient = parallax_coefficient;
    this->addObjects(objects_list);
}

void ObjectsLayer::deleteObject(Sprite *object)
{
    for(int i=0;i<objects.size();i++)
    {
        if (object == objects[i])
            objects.erase(objects.begin() + i);
    }
}

ObjectsLayer::~ObjectsLayer()
{
    for (int i=0; i<objects.size(); i++)
        delete objects[i];
    objects.clear();
}
