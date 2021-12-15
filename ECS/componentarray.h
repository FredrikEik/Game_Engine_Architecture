#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

//!
//! \brief The IComponentArray class,
//! Interface class which makes it possible to have different kinds of component arrays. A pure virtual class.
//!
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

//!
//! \brief The ComponentArray class,
//! Holds one type of components in an array. which component it holds depends on the type <T>.
//!
//! This is a template class which interacts with its respective components. It adds and remove components on the orders from the ComponentManager class.
//! One instance of this class represent one type of component.
//!
//! \see ComponentManager
//!
template<typename T>
class ComponentArray : public IComponentArray
{
public:

    //!
    //! \brief InsertComponent(), connects an entity to a component of type <T>
    //!
    //! Uses the entity to craft a new index and stores it in the entityToIndex map.
    //! Then it does the same with the indexToEntity map.
    //! Finally it inserts the component in the array at the assigned index.
    //!
    //! \param entity - The entity that gets the component.
    //! \param component - The type of component the entity gets.
    //!
    void InsertComponent(Entity entity, T component)
    {
        assert(m_entityToIndex.find(entity) == m_entityToIndex.end() && "Entity allready have a component of that type");

        //Add component at the end and update maps.
        size_t newIndex = m_size;
        m_entityToIndex[entity] = newIndex;
        m_IndexToEntity[newIndex] = entity;
        m_componentArray[newIndex] = component;
        m_size++;
    }

    //!
    //! \brief ClearComponent(), Removes a component of type <T> from the given entity.
    //!
    //! Since this class represent one type of components, it is implied that it is the given type of component that gets removed.
    //! The function also fills any gaps in the array to keep it as compact as possible. This helps to prevent cache misses on the CPU.
    //!
    //! \param entity - The entity that looses a component.
    //!
    void ClearComponent(Entity entity)
    {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Trying to remove non-exsisting component!");

        //Put the last component of the array into the spot where the removed componet was(is).
        size_t indexOfRemovedEntity = m_entityToIndex[entity];
        size_t indexOfLastEntity = m_size-1;
        m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastEntity];

        //Update and clean maps
        Entity entityOfLastElement = m_IndexToEntity[indexOfLastEntity];
        m_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

        m_entityToIndex.erase(entity);
        m_IndexToEntity.erase(indexOfLastEntity);

        m_size--;
    }

    //!
    //! \brief GetData(), gets the data from this instance's component type.
    //! \param entity - The component owning entity.
    //! \return Returns a reference to the entity's component.
    //!
    T& GetData(Entity entity)
    {
        assert(m_IndexToEntity.find(entity) != m_IndexToEntity.end() && "Trying to retrive a non-existing component!");

        //returning a reference to the entity component.
        return m_componentArray[m_entityToIndex[entity]];
    }

    //!
    //! \brief EntityDestroyed(), Clears the entity's component from the array.
    //! \param entity - The entity that gets deleted.
    //!
    //! Checks if the entity has a component of this type. If it does, it clears that component.
    //!
    //! \see ClearComponent()
    //!
    void EntityDestroyed(Entity entity) override //Virtual
    {
        if (m_entityToIndex.find(entity) != m_entityToIndex.end())
        {
            ClearComponent(entity);
        }
    }

private:
    //! An array of components of type <T>, Can not be more than MAX_ENTITIES.
    std::array<T, MAX_ENTITIES> m_componentArray;

    //! A map linking an entity to its index in the array.
    std::unordered_map<Entity, size_t> m_entityToIndex;

    //! A map linking an index to its entity.
    std::unordered_map<size_t, Entity> m_IndexToEntity;

    //! How big the array currently is.
    size_t m_size;
};


#endif // COMPONENTMANAGER_H
