#ifndef COMPMANAGER_H
#define COMPMANAGER_H

#include "componentarray.h"

/** @class ComponentManager
 * @brief The Component Manager class.
 * It's job is to keep track of all the different components that exist, aswell as adding and removing components.
 *
 * This class is one of the ECS managers. It is supposed to be a single instance only. Functions can be accessed from the ECS manager.
 *
 * \see ECScoordinator::RegisterComponent()
 * \see ECScoordinator::AddComponent()
 * \see ECScoordinator::RemoveComponent()
 */
class ComponentManager
{
public:
    //!
    //! \brief RegisterComponent()
    //! Registers a new component type for the first time, it creates a new ComponentArray for the appropriate type.
    //!
    //! Make sure to only register a component once.
    //!
    //! \typedef What kind of component you want to register.
    //! \see GetComponentType()
    //! \see AddComponent()
    //! \see RemoveComponent()
    //!
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

    //!
    //! \brief GetComponentType()
    //! Gets the components type.
    //!
    //! The type of component needs to be registered before this function can be used.
    //!
    //! \typedef T - The component type you want to get.
    //! \return The component <T>'s type as a GLubyte.
    //!
    template<typename T>
    ComponentType GetComponentType()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "The Component is not registered before being used!");

        //Return the component type of typeName, this is used for creating signatures!
        return m_ComponentTypes[typeName];
    }

    //!
    //! \brief AddComponent()
    //! Inserts an entity's component <T> into the corresponding component array <T>.
    //!
    //! \param entity - The components owner.
    //! \param component - The component you want to give an entity.
    //!
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertComponent(entity, component);
    }

    //!
    //! \brief RemoveComponent()
    //! Removes an entity's component <T>.
    //!
    //! \param entity - The entity that needs removing a component.
    //!
    //! \typedef T - The component type that needs removing.
    //!
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->ClearComponent(entity);
    }

    //!
    //! \brief GetComponent()
    //! Gets you a spesific component from the component array.
    //!
    //! \param entity - The entity that owns the component.
    //!
    //! \typedef T - The component type you want to get.
    //!
    template<typename T>
    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    //!
    //! \brief DestroyEntity()
    //! Destroys the entity by deleting the components associated with it.
    //!
    //! Loops over all the components and deletes them from each array.
    //!
    //! \param entity - The entity that gets deleted.
    //!
    //! \see ComponentArray::EntityDestroyed()
    //!
    void DestroyEntity(Entity entity)
    {
        for (const auto &pair : m_ComponentArrays)
        {
            const auto &component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

private:
    //! Map of pointers to the different component types.
    std::unordered_map<const char*, ComponentType> m_ComponentTypes{};

    //!Map of pointers to the component arrays.
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

    //!The component type for the next registered component, starting at 0.
    ComponentType m_NextComponentType{0};

    //!
    //! \brief GetComponentArray()
    //! A way to get a spesific component array <T>.
    //!
    //! Used in GetComponenet().
    //!
    //! \typedef T - What kind of component array you need.
    //!
    //! \see GetComponent()
    //!
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "The component used is not registered!");

        return std::static_pointer_cast<T>(m_ComponentArrays[typeName]);
    }
};

#endif // COMPMANAGER_H
