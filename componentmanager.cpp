#include "componentmanager.h"

ComponentManager::ComponentManager()
{
}

template<typename T>
void ComponentManager::RegisterComponent()
{
    const char* typeName = typeid(T).name();

    assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

    //Insert new componenttype into map, then iterate
    mComponentTypes.insert({typeName, mNextComponentType});
    ++mNextComponentType;

    //Make new componentarray and put it in the map with a string
    mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

}

template<typename T>
void ComponentManager::AddComponent(gsl::Entity entity, T component)
{
    // Add a component to the array for an entity
    GetComponentArray<T>()->InsertData(entity, component);
}

template<typename T>
gsl::ComponentType ComponentManager::GetComponentType()
{
    const char* typeName = typeid(T).name();

    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    // Return this component's type - used for creating signatures
    return mComponentTypes[typeName];
}

template<typename T>
void ComponentManager::RemoveComponent(gsl::Entity entity)
{
    // Remove a component from the array for an entity
    GetComponentArray<T>()->RemoveData(entity);
}

template<typename T>
T& ComponentManager::GetComponent(gsl::Entity entity)
{
    // Get a reference to a component from the array for an entity
    return GetComponentArray<T>()->GetData(entity);
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

template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
    const char* typeName = typeid(T).name(); //Get component name

    //Throw an error if componenttype is not registered
    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
}
