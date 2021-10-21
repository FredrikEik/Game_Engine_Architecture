#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"

class PhysicsSystem
{
public:
    PhysicsSystem();
    void freeFall(float deltaTime, TransformComponent* Transf, float radius);
    void bounce_floor(float deltaTime, TransformComponent* Transf, float radius);
    bool once = true;
};

#endif // PHYSICSSYSTEM_H
