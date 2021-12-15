#pragma once
#include "../CoreMinimal.h"
#include <array>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "../DataStructures/TArray.h"


/// <summary>
/// It exists one ComponentManager per component type. 
/// This should never be instantiated outside of factory, but it is fine to get it.
/// </summary>
/// <detailed>
/// There should only ever exist one class per component type. Therefore, do NOT instantiate this directly. Go through Factory
/// The class exists as a way to manage all the components of a given type.
/// The intention is to make lookups very fastand easy
/// Drawbacks : It is using a lot of memory, generatingand array the size of MAX_ENTITIES * 4byte per component type, in addition to all the components.
/// One could use an unordered map instead to reduce the memory footprint at the cost of speed
/// </detailed>
template<class T>
class ComponentManager
{
	friend class Factory;
	ComponentManager() :
		sparseComponentArray{ (*new std::array<uint32, core::MAX_ENTITIES>{}) },
		packedComponentArray{ (*new std::vector<T>) },
		componentSize{sizeof(T)}
	{
		for (auto &it: sparseComponentArray)
			it=core::MAX_ENTITIES+1;
		packedComponentArray.reserve(50);
	}
	~ComponentManager();
public:
	/// <summary>
	/// UNSAFE Gets a reference to the entity's component.
	/// Fine to use when you know the component is valid
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <returns>reference to the entity's component.</returns>
	T& getComponent(uint32 entityID);

	/// <summary>
	/// SAFE Returns a pointer to the entity's component.
	/// Checks if the component exists 
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <returns>Pointer to component if it exists, nullptr otherwise</returns>
	T* getComponentChecked(uint32 entityID);

	/// <summary>
	/// Returns a component based on componentID. Slow, so only use in situations where you don't know the entity.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns>Pointer to the Component of the ComponentManagers component type</returns>
	T* getComponentFromID(uint32 componentID);

	/// <summary>
	/// Gets every component of this type.
	/// Use this whenever you are iterating over all/most of one component type.
	/// E.g perfect for rendering. Get all MeshComponents and iterate over them
	/// </summary>
	/// <returns>Vector containing every component of this type. Guaranteed valid</returns>
	std::vector<T>& getComponentArray();

	// Shrink the packedComponentArray if it is dirty
	void cleanUp();
protected:

	/// <summary>
	/// Creates a new component, adding it to a contiguous array.
	/// Keeps the index to the component in a sparse array for O(1) lookup
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="isReusable">if set to <c>true</c> [is reusable].</param>
	/// <returns>The component ID of the new component</returns>
	uint32 createComponent(uint32 entityID, bool isReusable);


	/// <summary>
	/// Assigns a component to a new entity.
	/// </summary>
	/// <param name="componentID">The component identifier.</param>
	/// <param name="entityID">The entity identifier.</param>
	/// <returns>True if the component exists and was assigned</returns>
	bool assignComponent(uint32 componentID, uint32 entityID);

	/// <summary>
	/// Remove a component. This does not shrink the packedComponentArray, so the memory will still be reserved. 
	/// Call cleanUp when there is time available to free the memory.
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <returns>the component ID of the removed component</returns>
	uint32 removeComponent(uint32 entityID);

private:
	/* This array contains the location in the packedComponentArray
	for a specific entity's component*/

	/// <summary>
	/// The sparse component array. Contains the index to an entity's component in the packedComponentArray at the EntityID index
	/// </summary>
	std::array<uint32, core::MAX_ENTITIES>& sparseComponentArray;

	/// <summary>
	/// The packed component array. Containing all the components of a this component type. 
	/// </summary>
	std::vector<T>& packedComponentArray;
	bool packedComponentDirty{ false };

	bool bIsReusable{ false };

	std::size_t componentSize{};
};

template<class T>
inline ComponentManager<T>::~ComponentManager()
{
	delete &sparseComponentArray;
	delete &packedComponentArray;
}

template<class T>
inline uint32 ComponentManager<T>::createComponent(uint32 entityID, bool isReusable)
{
	assert(entityID < core::MAX_ENTITIES);

	bIsReusable = isReusable;

	// Assigning the components location in the packed array to the sparse array
	sparseComponentArray[entityID] = packedComponentArray.size();

	// Constructing a new T and pushing it to the packed array
	// The array is already on the heap, so we don't need to use new. 
	// This way, the objects will be in a contiguous array
	packedComponentArray.push_back(T(entityID, sparseComponentArray[entityID]));


	return packedComponentArray.back().ID;
}

template<class T>
inline bool ComponentManager<T>::assignComponent(uint32 componentID, uint32 entityID)
{
	if (componentID < packedComponentArray.size())
	{
		sparseComponentArray[entityID] = componentID;
		return true;
	}

	return false;
}

template<class T>
inline uint32 ComponentManager<T>::removeComponent(uint32 entityID)
{
	assert(entityID < core::MAX_ENTITIES);
	uint32 positionInPacked{ sparseComponentArray[entityID] };
	sparseComponentArray[entityID] = core::MAX_ENTITIES + 1; // Invalidating the component lookup.

	// If the component is not the last element in the vector, overwrite it with the last element
	// This works because the order in the packed array does not matter.
	if (positionInPacked != packedComponentArray.size() - 1)
	{
		std::cout << "Removing component "<<typeid(T).name() <<" from entityID " << packedComponentArray[positionInPacked].entityID << '\n';

		packedComponentArray[positionInPacked] = packedComponentArray[packedComponentArray.size() - 1];

		uint32 swappedEntityID = packedComponentArray[packedComponentArray.size() - 1].entityID;
		sparseComponentArray[swappedEntityID] = positionInPacked;
	}

	// Remove the component and mark the vector dirty.
	uint32 componentID = packedComponentArray.back().ID;
	assert(packedComponentArray.size() > 0);
	packedComponentArray.pop_back();
	packedComponentDirty = true;
	return componentID;
}

template<class T>
inline T& ComponentManager<T>::getComponent(uint32 entityID)
{
	assert(entityID >= 0 && sparseComponentArray[entityID] < packedComponentArray.size());
	return packedComponentArray[sparseComponentArray[entityID]];
}

template<class T>
inline T* ComponentManager<T>::getComponentChecked(uint32 entityID)
{
	if(entityID >= 0 && sparseComponentArray[entityID] < packedComponentArray.size())
		return &packedComponentArray[sparseComponentArray[entityID]];
	return nullptr;
}

template<class T>
inline T* ComponentManager<T>::getComponentFromID(uint32 componentID)
{
	for (auto& it : packedComponentArray)
	{
		if (it.ID == componentID)
			return &it;
	}

	return nullptr;
}

template<class T>
inline std::vector<T>& ComponentManager<T>::getComponentArray()
{
	return (packedComponentArray);
}

template<class T>
inline void ComponentManager<T>::cleanUp()
{
	if (packedComponentDirty)
	{
		packedComponentArray.shrink_to_fit();
		packedComponentDirty = false;
	}
}
