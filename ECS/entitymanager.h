#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>
#include <bitset>
#include "components.h"

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager
{
    friend class ECScoordinator;

public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

private:
    EntityManager();

    static std::unique_ptr<EntityManager> Create()
    { return std::unique_ptr<EntityManager>( new EntityManager() ); }

    //Holds availeble Id's for entitys to use.
    std::queue<Entity> m_availableEntityIDs;

    //Holds the signatures of the existing enteties.
    std::array<Signature, MAX_ENTITIES> m_signatures;

    //A simple counter used to determined if we went above the maximum allowed enteties.
    Entity m_livingEntitiyCounter{0};
};

#endif // ENTITYMANAGER_H
