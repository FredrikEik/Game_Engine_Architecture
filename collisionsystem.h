#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "collisionshapes.h"
#include <vector>

class CollisionSystem
{
public:
    CollisionSystem();
    void addShape(CollisionShape* shape);
    bool collisionTestPvsS();
    bool collisionTestSvsS();

private:
    std::vector<CollisionShapes*> m_Collidables; // Collidables in the world
    // contact informasjon
};

#endif // COLLISIONSYSTEM_H
