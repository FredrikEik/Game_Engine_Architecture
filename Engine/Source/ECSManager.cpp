#include "ECSManager.h"
#include "Factory.h"
#include <iostream>
ECSManager::ECSManager()
	: entities{ (*new std::array<std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, core::MAX_ENTITIES>) },
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
	//delete& factory;
	delete& entities;
	//delete& availableEntityIDs;
}


uint32 ECSManager::newEntity()
{
	uint32 entityID = availableEntityIDs.back();
	availableEntityIDs.pop_back();
	assert(!entities[entityID].first); // Trying to assign an entity that is already in use.

	std::pair<bool, std::vector<std::pair<std::type_index, uint32>>> entity(true, 
								std::vector<std::pair<std::type_index, uint32>>());
	entities[entityID] = entity;

	return entityID;
}



void ECSManager::destroyEntity(uint32 entityID)
{
	std::vector<std::pair<std::type_index, uint32>> components = entities[entityID].second;
	for (uint32 i{}; i < components.size(); ++i)
	{
		factory.removeComponent(entityID, components[i].first, components[i].second); 
	}
	entities[entityID].second.clear();
	entities[entityID].first = false;
	availableEntityIDs.push_back(entityID);
}

void ECSManager::printEntity(uint32 entityID)
{
	std::vector<std::pair<std::type_index, uint32>> components = entities[entityID].second;
	std::cout << "Entity id: " << entityID << ". Active: " << entities[entityID].first << '\n'
		<<"Number of components: "<<components.size()<<'\n';
	for (uint32 i{}; i < components.size(); ++i)
	{
		std::cout << components[i].first.name() << '\n';
	}

}

uint32 ECSManager::loadAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	return factory.loadAsset(entityID, filePath);
}

uint32 ECSManager::loadAsset(uint32 entityID, enum DefaultAsset defaultAsset)
{
	return factory.loadAsset(entityID, defaultAsset);
}