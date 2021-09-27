#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>
#include <array>
#include "constants.h"
#include "assert.h"

// uint16_t can be used up to the value of 65535
// As long as our amount of entities is less than this then we will be fine

class EntityManager
{
public:
    EntityManager();

    gsl::Entity CreateEntity();

    void DestroyEntity(gsl::Entity entity);

    void SetSignature(gsl::Entity entity, gsl::Signature signature);

    gsl::Signature GetSignature(gsl::Entity entity);

private:
    uint32_t mLivingEntitiesCount;

    std::array<gsl::Signature, gsl::MAX_ENTITIES> mSignatures{};

    std::queue<gsl::Entity> mAvailableEntities;
};

#endif // ENTITYMANAGER_H
