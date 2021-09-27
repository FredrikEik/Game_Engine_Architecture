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



#endif // COMPONENTMANAGER_H
