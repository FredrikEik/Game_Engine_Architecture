#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>

// uint16_t can be used up to the value of 65535
// As long as our amount of entities is less than this then we will be fine
typedef uint16_t Entity;

const Entity MAX_ENTITIES = 10000;

class EntityManager
{
public:
    EntityManager();

private:
    int mLivingEntitiesCount;

    std::queue<Entity> mAvailableEntities;
};

#endif // ENTITYMANAGER_H
