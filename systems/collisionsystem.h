#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "components.h"

class collisionSystem
{
public:
    collisionSystem();
    bool isColliding(MeshComponent *Mesh1,TransformComponent* transform1, MeshComponent* Mesh2, TransformComponent* transform2);
};

#endif // COLLISIONSYSTEM_H
