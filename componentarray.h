#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <components.h>

//TODO: Try to make events work so that virtual functions is not needed.
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    //ComponentManager();

    void AddComponent(Entity entity, T component)
    {
        assert(m_entityToIndex.find(entity) == m_entityToIndex.end() && "Entity allready have a component of that type");

        //Add component at the end and update maps.
        size_t newIndex = m_size;
        m_entityToIndex[entity] = newIndex;
        m_IndexToEntity[newIndex] = entity;
        m_componentArray[newIndex] = component;
        m_size++;
    }

    void RemoveComponent(Entity entity)
    {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Trying to remove non-exsisting component!");

        //Put the last component of the array into the spot where the removed componet was(is).
        size_t indexOfRemovedEntity = m_entityToIndex[entity];
        size_t indexOfLastEntity = m_size-1;
        m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastEntity];

        //Update maps
        Entity entityOfLastElement = m_IndexToEntity[indexOfLastEntity];
        m_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

        m_entityToIndex.erase(entity);
        m_IndexToEntity.erase(indexOfLastEntity);

        m_size--;
    }

    T& GetData(Entity entity)
    {
        assert(m_IndexToEntity.find(entity) != m_IndexToEntity.end() && "Trying to retrive an non-existing component!");

        //returning a reference to the entity component.
        return m_componentArray[m_entityToIndex[entity]];
    }

    void EntityDestroyed(Entity entity) override //Virtual
    {
        if (m_entityToIndex.find(entity) != m_entityToIndex.end())
        {
            RemoveComponent(entity);
        }
    }

private:
    //TODO: Find a better solution than maps?
    std::array<T, MAX_ENTITIES> m_componentArray;

    std::unordered_map<Entity, size_t> m_entityToIndex;

    std::unordered_map<size_t, Entity> m_IndexToEntity;

    size_t m_size;
};

#endif // COMPONENTMANAGER_H
