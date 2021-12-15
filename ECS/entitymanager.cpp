#include "entitymanager.h"

//!
//! \brief EntityManager::EntityManager, constructor. Populates the entity ID que.
//!
EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
    {
        m_availableEntityIDs.push(entity);
    }
}

//!
//! \brief EntityManager::CreateEntity, Creates a new entity.
//! \return Returns an entity's ID (GLuint).
//!
//! Assigns the ID from the que of availible ID's. Also counts the number of living entities.
//!
Entity EntityManager::CreateEntity()
{
    assert(m_livingEntitiyCounter < MAX_ENTITIES && "Too many enteties created!");

    Entity id = m_availableEntityIDs.front();
    m_availableEntityIDs.pop();
    m_livingEntitiyCounter++;

    return id;
}

//!
//! \brief EntityManager::DestroyEntity, Deletes the entity's signature and frees up the ID for future use.
//! \param entity - The entitiy that gets deleted.
//!
//! Updates living entity counter.
//!
void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    m_signatures[entity].reset(); //Invalidate the entity's signature.

    m_availableEntityIDs.push(entity); //Make ID availeble for other enteties again.

    m_livingEntitiyCounter--;
}

//!
//! \brief EntityManager::SetSignature, Sets the signature of an entity.
//! \param entity - The entity you want to give a signature.
//! \param signature - The signature of the entity.
//!
//! Stores the signature in the signature array.
//!
void EntityManager::SetSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    m_signatures[entity] = signature;
}

//!
//! \brief EntityManager::GetSignature, Gets the signature of an entity.
//! \param entity - The entity you want the signature of.
//! \return The signature of an entity.
//!
Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Etity out of range!");

    return m_signatures[entity];
}
