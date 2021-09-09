#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>
#include <bitset>
#include "components.h"

using Entity = GLuint;
const Entity MAX_ENTITIES = 1000;

using ComponentType = GLubyte;
const ComponentType MAX_COMPONENTS = 16;

using Signature = std::bitset<MAX_COMPONENTS>;


class EntityManager
{
public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

private:
    //TODO: Friend with factory class!
    //friend class Factory;
    EntityManager();

    //Holds availeble Id's for entitys to use.
    std::queue<Entity> m_availableEntityIDs;

    std::array<Signature, MAX_ENTITIES> m_signatures;

    Entity m_livingEntitiyCounter{0};
};

#endif // ENTITYMANAGER_H
