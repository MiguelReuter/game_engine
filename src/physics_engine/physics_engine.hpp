#ifndef physics_hpp
#define physics_hpp

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "circle_dynamic_body.hpp"
#include "dynamic_body.hpp"
#include "static_body.hpp"

using namespace std;


class PhysicsEngine
{
public:
    static PhysicsEngine* Instance();
    
    // dynamic body
    vector<DynamicBody*> dynamic_bodies;
    void addDynamicBody(DynamicBody* body){
        if (body->isPrimaryDynamic())
            primary_dynamic_bodies.push_back(body);
        else
            dynamic_bodies.push_back(body);
    };
    
    // primary dynamic body
    vector<DynamicBody*> primary_dynamic_bodies;
    
    // circle dynamic body
    vector<CircleDynamicBody*> circle_dynamic_bodies;
    void addCircleDynamicBody(CircleDynamicBody* body){circle_dynamic_bodies.push_back(body);};
    
    // static body
    vector<StaticBody*> static_bodies;
    void addStaticBody(StaticBody* body){static_bodies.push_back(body);};
    void addStaticBodies(vector<StaticBody*> bodies){static_bodies.insert(static_bodies.end(), bodies.begin(), bodies.end());};
    
    void deleteObject(Body* object);
    
    void update();
    void quit();
    
private:
    PhysicsEngine();
    PhysicsEngine(PhysicsEngine const&);
    PhysicsEngine& operator=(PhysicsEngine const&);
    static PhysicsEngine* m_instance;
};


#endif /* physics_hpp */
