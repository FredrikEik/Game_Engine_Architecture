#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "components.h"

class PhysicsSystem
{
public:
    PhysicsSystem();
    void freeFall(long deltaTime, TransformComponent* Transf);
};

#endif // PHYSICSSYSTEM_H
