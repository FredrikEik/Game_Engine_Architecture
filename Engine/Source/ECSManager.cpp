#include "ECSManager.h"
#include "Factory.h"
#include <iostream>
ECSManager::ECSManager()
	: entities{ (*new std::array<std::pair<uint32, std::vector<std::pair<std::type_index, uint32>>>, core::MAX_ENTITIES>) },
	factory{*new Factory},
	availableEntityIDs{*new std::vector<uint32>}
{
	availableEntityIDs.reserve(sizeof(uint32)* core::MAX_ENTITIES);
	// Populating the available entityIDs with 0 at the back. 
	// This is just so the first entity we create gets ID 0 due to pop_back
	for (uint32 i{}; i < core::MAX_ENTITIES; ++i)
		availableEntityIDs.push_back(core::MAX_ENTITIES - i - 1);
}

ECSManager::~ECSManager()
{
	delete& factory;
	delete& entities;
	delete& availableEntityIDs;
}


uint32 ECSManager::newEntity()
{
	uint32 entityID = availableEntityIDs.back();
	availableEntityIDs.pop_back();
	std::pair<uint32, std::vector<std::pair<std::type_index, uint32>>> entity(entityID, std::vector<std::pair<std::type_index, uint32>>());

	return entityID;
}

void ECSManager::destroyEntity(uint32 entityID)
{
	std::vector<std::pair<std::type_index, uint32>> components = entities.at(entityID).second;
	for (uint32 i{}; i < components.size(); ++i)
	{
		// TODO: Add a remove in factory that takes type_index instead of <T>
		factory.removeComponent() // Purposeful compile error
	}
}