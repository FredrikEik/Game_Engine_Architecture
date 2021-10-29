#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "componenttypes.h"
#include "componentarray.h"
#include "constants.h"
#include "assert.h"

class ComponentManager
{
public:
    ComponentManager();

    template<typename T>
    void RegisterComponent();

    template<typename T>
    void AddComponent(gsl::Entity entity, T component);

    template<typename T>
    gsl::ComponentType GetComponentType();

    template<typename T>
    void RemoveComponent(gsl::Entity entity);

    template<typename T>
    T& GetComponent(gsl::Entity entity);

    void EntityDestroyed(gsl::Entity entity);

private:

    //Mapping string to component type, holds all component types
    std::unordered_map<const char* , gsl::ComponentType> mComponentTypes{};

    //Mapping string to component array, holds all component arrays
    std::unordered_map<const char*, std::shared_ptr<ParentComponentArray>> mComponentArrays{};

    gsl::ComponentType mNextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray();
};

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


template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
    const char* typeName = typeid(T).name(); //Get component name

    //Throw an error if componenttype is not registered
    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
}




#endif // COMPONENTMANAGER_H
