#include "physics_engine.hpp"


using namespace std;

PhysicsEngine* PhysicsEngine::m_instance = nullptr;


PhysicsEngine* PhysicsEngine::Instance()
{
    if (!m_instance)
        m_instance = new PhysicsEngine();
    return m_instance;
}


PhysicsEngine::PhysicsEngine()
{
    //cout << "Physics engine created" << endl;
}

void PhysicsEngine::update()
{
    // vector : dynamic_bodies + circle_dynamic_bodies + primary_dynamic_bodies
    vector<DynamicBody*> all_dynamic_bodies (dynamic_bodies.begin(), dynamic_bodies.end());
    all_dynamic_bodies.insert(all_dynamic_bodies.end(), circle_dynamic_bodies.begin(), circle_dynamic_bodies.end());
    all_dynamic_bodies.insert(all_dynamic_bodies.end(), primary_dynamic_bodies.begin(), primary_dynamic_bodies.end());


    for (unsigned int i=0; i<all_dynamic_bodies.size(); i++)
        all_dynamic_bodies[i]->update(static_bodies, dynamic_bodies);

    // reset
    for (unsigned int i=0; i<all_dynamic_bodies.size(); i++) {
        all_dynamic_bodies[i]->force[0] = .0;
        all_dynamic_bodies[i]->force[1] = .0;
    }

}


void PhysicsEngine::deleteObject(Body *object)
{
    // dynamic_bodies
    for (unsigned int i=0; i<dynamic_bodies.size(); i++) {
        if (dynamic_bodies[i] == object)
            dynamic_bodies.erase(dynamic_bodies.begin() + i);
        if(dynamic_bodies[i]->getOnBody() == object)
            dynamic_bodies[i]->setOnBody(nullptr);
    }

    // primary dynamic_bodies
    for (unsigned int i=0; i<primary_dynamic_bodies.size(); i++) {
        if (primary_dynamic_bodies[i] == object)
            primary_dynamic_bodies.erase(primary_dynamic_bodies.begin() + i);
        if(primary_dynamic_bodies[i]->getOnBody() == object)
            primary_dynamic_bodies[i]->setOnBody(nullptr);
    }

    // circle_dynamic_bodies
    for (unsigned int i=0; i<circle_dynamic_bodies.size(); i++) {
        if (circle_dynamic_bodies[i] == object)
            circle_dynamic_bodies.erase(circle_dynamic_bodies.begin() + i);
        if(circle_dynamic_bodies[i]->getOnBody() == object)
            circle_dynamic_bodies[i]->setOnBody(nullptr);
    }

    // static_bodies
    for (unsigned int i=0; i<static_bodies.size(); i++) {
        if (static_bodies[i] == object)
            static_bodies.erase(static_bodies.begin() + i);
    }
}



void PhysicsEngine::quit()
{
    // dynamic_bodies
    dynamic_bodies.clear();

    // circle_dynamic_bodies
    circle_dynamic_bodies.clear();

    // static_bodies
    static_bodies.clear();
};
