#pragma once
#include "../CoreMinimal.h"
#include <array>
#include <vector>
#include <assert.h>
#include <algorithm>
/* There should only ever exist one class per component type. 
* The class exists as a way to manage all the components of a given type. 
* The intention is to make lookups very fast and easy
* Drawbacks: It is using a lot of memory, generating and array the size of MAX_ENTITIES*4byte per component type, in addition to all the components.
* One could use an unordered map instead to reduce the memory footprint at the cost of speed.*/
template<class T>
class ComponentManager
{
public:
	ComponentManager() :
		sparseComponentArray{ (*new std::array<uint32, core::MAX_ENTITIES>{}) },
		packedComponentArray{ (*new std::vector<T>) }
	{
		for (auto &it: sparseComponentArray)
			it=-1;
	}
	~ComponentManager();

	uint32 createComponent(uint32 entityID);
	bool removeComponent(uint32 entityID);

	T getComponent(uint32 entityID);

	std::vector<T>& getComponentArray();
private:
	/* This array contains the location in the packedComponentArray
	for a specific entity's component*/
	std::array<uint32, core::MAX_ENTITIES>& sparseComponentArray;
	// This array contains all the components
	std::vector<T>& packedComponentArray;

};

template<class T>
inline ComponentManager<T>::~ComponentManager()
{
	delete &sparseComponentArray;
	delete &packedComponentArray;
}

template<class T>
inline uint32 ComponentManager<T>::createComponent(uint32 entityID)
{
	assert(entityID < core::MAX_ENTITIES);
	packedComponentArray.push_back(T(entityID));
	uint32 componentID = packedComponentArray.back().ID;
	sparseComponentArray.at(entityID) = componentID;
	
	return componentID;
}

template<class T>
inline bool ComponentManager<T>::removeComponent(uint32 entityID)
{
	assert(entityID < core::MAX_ENTITIES);
	uint32 positionInPacked{ sparseComponentArray.at(entityID) };
	sparseComponentArray.at(entityID) = -1; // Invalidating the component lookup

	// If the component is not the last element in the vector, swap it with the last element
	// This works because the order in the packed array does not matter.
	if (positionInPacked != packedComponentArray.size() - 1)
	{
		packedComponentArray.at(positionInPacked) = packedComponentArray[packedComponentArray.size() - 1];

		uint32 swappedEntityID = packedComponentArray[positionInPacked].entityID;

		sparseComponentArray[swappedEntityID] = positionInPacked;
	}

	packedComponentArray.pop_back();
	return true;
}

template<class T>
inline T ComponentManager<T>::getComponent(uint32 entityID)
{
	assert(entityID >= 0 && entityID < packedComponentArray.size());
	return packedComponentArray.at(sparseComponentArray.at(entityID));
}

template<class T>
inline std::vector<T>& ComponentManager<T>::getComponentArray()
{
	return (packedComponentArray);
}
