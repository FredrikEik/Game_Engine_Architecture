#include "entitymanager.h"

EntityManager::EntityManager()
{
    // Initialize the queue with all possible entity IDs
    for (gsl::Entity entity = 0; entity < gsl::MAX_ENTITIES; ++entity)
    {
        mAvailableEntities.push(entity);
    }
}

gsl::Entity EntityManager::CreateEntity()
{
    assert(mLivingEntitiesCount < gsl::MAX_ENTITIES && "Too many entities in existence.");

    // Take an ID from the front of the queue
    gsl::Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntitiesCount;

    return id;
}

void EntityManager::DestroyEntity(gsl::Entity entity)
{
    assert(entity < gsl::MAX_ENTITIES && "Entity out of range.");

    // Invalidate the destroyed entity's signature
    mSignatures[entity].reset();

    // Put the destroyed ID at the back of the queue
    mAvailableEntities.push(entity);
    --mLivingEntitiesCount;
    }

void EntityManager::SetSignature(gsl::Entity entity, gsl::Signature signature)
{
    assert(entity < gsl::MAX_ENTITIES && "Entity out of range.");

    // Put this entity's signature into the array
    mSignatures[entity] = signature;
}

gsl::Signature EntityManager::GetSignature(gsl::Entity entity)
{
    assert(entity < gsl::MAX_ENTITIES && "Entity out of range.");

    // Get this entity's signature from the array
    return mSignatures[entity];
}
