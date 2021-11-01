#include "entitymanager.h"

EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
    {
        m_availableEntityIDs.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    assert(m_livingEntitiyCounter < MAX_ENTITIES && "Too many enteties created!");

    Entity id = m_availableEntityIDs.front();
    m_availableEntityIDs.pop();
    m_livingEntitiyCounter++;

    return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    m_signatures[entity].reset(); //Invalidate the entity's signature.

    m_availableEntityIDs.push(entity); //Make ID availeble for other enteties again.

    m_livingEntitiyCounter--;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    m_signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    return m_signatures[entity];
}
