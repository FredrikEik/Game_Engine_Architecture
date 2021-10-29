#include "componentmanager.h"

ComponentManager::ComponentManager()
{
}


void ComponentManager::EntityDestroyed(gsl::Entity entity)
{
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto const& pair : mComponentArrays)
    {
        auto const& component = pair.second;

        component->EntityDestroyed(entity);
    }
}
