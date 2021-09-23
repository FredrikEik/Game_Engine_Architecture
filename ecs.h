#ifndef ECS_H
#define ECS_H

#include "componentmanager.h"
#include "entitymanager.h"
#include "systemmanager.h"


class ECS
{
public:
    ECS();

private:

    EntityManager* EntityManager;
    ComponentManager* ComponentManager;
    SystemManager* SystemManager;

};

#endif // ECS_H
