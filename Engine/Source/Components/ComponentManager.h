#pragma once
#include "../CoreMinimal.h"
#include <array>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "../DataStructures/TArray.h"
/* There should only ever exist one class per component type. Therefore, do NOT instantiate this directly. Go through Factory
* The class exists as a way to manage all the components of a given type. 
* The intention is to make lookups very fast and easy
* Drawbacks: It is using a lot of memory, generating and array the size of MAX_ENTITIES*4byte per component type, in addition to all the components.
* One could use an unordered map instead to reduce the memory footprint at the cost of speed.*/
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
	T& getComponent(uint32 entityID);
	T* getComponentChecked(uint32 entityID);
	/// <summary>
	/// Returns a component based on componentID. Slow, so only use in situations where you don't know the entity.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns>Pointer to the Component of the ComponentManagers component type</returns>
	T* getComponentFromID(uint32 componentID);
	std::vector<T>& getComponentArray();

	// Shrink the packedComponentArray if it is dirty
	void cleanUp();
protected:
//public:
	uint32 createComponent(uint32 entityID, bool isReusable);
	bool assignComponent(uint32 componentID, uint32 entityID);
	/*@returns the component ID of the removed component
	*Remove a component. This does not shrink the packedComponentArray, so the memory will still be reserved. 
	* Call cleanUp when there is time available to free the memory.*/
	uint32 removeComponent(uint32 entityID);

private:
	/* This array contains the location in the packedComponentArray
	for a specific entity's component*/
	std::array<uint32, core::MAX_ENTITIES>& sparseComponentArray;
	// This array contains all the components
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
	//std::cout << "Creating component " << typeid(T).name() << ". Size of packedArray before push: " << packedComponentArray.size()<<'\n';
	packedComponentArray.push_back(T(entityID, sparseComponentArray[entityID]));

	//std::cout << "Creating component " << typeid(T).name() << ". Size of packedArray after push: " << packedComponentArray.size()<<"\n\n";
	//std::cout << "Size of created component: " << sizeof(T) << '\n';
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
	packedComponentArray.pop_back(); // Does not fuck up without this
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
