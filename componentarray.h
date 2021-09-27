#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H


#include "constants.h"
#include <unordered_map>
#include "assert.h"

class ParentComponentArray
{
public:
    virtual ~ParentComponentArray() = default;
    virtual void EntityDestroyed(gsl::Entity entity) = 0;
};

template<typename T>
class ComponentArray : public ParentComponentArray
{

public:

    void EntityDestroyed(gsl::Entity entity) override
        {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
            {
                // Remove the entity's component if it existed
                RemoveData(entity);
            }
        }

    void InsertData(gsl::Entity entity, T component)
        {
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

            // Put new entry at end and update the maps
            size_t newIndex = mSize;
            mEntityToIndexMap[entity] = newIndex;
            mIndexToEntityMap[newIndex] = entity;
            mComponentArray[newIndex] = component;
            ++mSize;
        }


    void RemoveData(gsl::Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
        size_t indexOfLastElement = mSize - 1;
        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

        // Update map to point to moved spot
        gsl::Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
        mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        mEntityToIndexMap.erase(entity);
        mIndexToEntityMap.erase(indexOfLastElement);

        --mSize;
    }

    T& GetData(gsl::Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

        // Return a reference to the entity's component
        return mComponentArray[mEntityToIndexMap[entity]];
    }
private:
    std::array<T, gsl::MAX_ENTITIES> mComponentArray;

    // Map from an entity ID to an array index.
    std::unordered_map<gsl::Entity, size_t> mEntityToIndexMap;

    // Map from an array index to an entity ID.
    std::unordered_map<size_t, gsl::Entity> mIndexToEntityMap;

    size_t mSize;
};

#endif // COMPONENTARRAY_H
