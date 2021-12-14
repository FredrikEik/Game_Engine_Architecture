#ifndef COMPMANAGER_H
#define COMPMANAGER_H


#include <typeinfo>
#include "componentarray.h"
#include "components.h"

class ComponentManager
{
public:
    template<typename T>
    void RegisterComponent()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Tried to register same component more than once!");

        //Add this component type to the map.
        m_ComponentTypes.insert({typeName, m_NextComponentType});

        //Create a ComponentArray pointer, then add it to the array map.
        m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        //Increment next component type value.
        m_NextComponentType++;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "The Component is not registered before being used!");

        //Return the component type of typeName, this is used for creating signatures!
        return m_ComponentTypes[typeName];
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertComponent(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->ClearComponent(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    void DestroyEntity(Entity entity)
    {
        for (const auto &pair : m_ComponentArrays)
        {
            const auto &component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

private:
    //Map of pointers to the different component types.
    std::unordered_map<const char*, ComponentType> m_ComponentTypes{};

    //Map of pointers to the component arrays.
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

    //The component type for the next registered component, starting at 0.
    ComponentType m_NextComponentType{0};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "The component used is not registered!");

        return std::static_pointer_cast<T>(m_ComponentArrays[typeName]);
    }
};

#endif // COMPMANAGER_H
