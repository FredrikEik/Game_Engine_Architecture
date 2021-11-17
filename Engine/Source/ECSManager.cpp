#include "ECSManager.h"
#include <iostream>
#include "Assets/DefaultAssets.h"

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
	delete& factory;
	delete& entities;
	delete& availableEntityIDs;
}

int32 ECSManager::getNumberOfEntities() const
{
	return core::MAX_ENTITIES - availableEntityIDs.size();
}

bool ECSManager::entityExists(int32 entityID) const
{
	return entities[entityID].first;
}

std::vector<std::pair<std::type_index, uint32>> ECSManager::getEntity(int32 entityID)
{
	assert(entities[entityID].first);
	return entities[entityID].second;
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
	if (!entities[entityID].first)
		return;

	std::vector<std::pair<std::type_index, uint32>> components = entities[entityID].second;
	for (uint32 i{}; i < components.size(); ++i)
	{
		//factory.removeComponent(entityID, components[i].first, components[i].second); 
		removeComponentByRTTI(entityID, components[i].first);
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
	// TODO: Add the component to the entity
	//return factory.loadAsset(entityID, filePath);

	uint32 componentID = factory.loadAsset(entityID, filePath);

	std::pair<std::type_index, uint32> componentLocation(getAssetTypeIndex(filePath), componentID);

	entities.at(entityID).second.push_back(componentLocation);

	return componentID;
}

uint32 ECSManager::loadAsset(uint32 entityID, DefaultAsset defaultAsset)
{
	uint32 componentID = factory.loadAsset(entityID, defaultAsset);

	std::pair<std::type_index, uint32> componentLocation(getAssetTypeIndex(defaultAsset), componentID);

	entities.at(entityID).second.push_back(componentLocation);

	return componentID;
}

Factory::ReusableAsset ECSManager::getReusableAsset(std::size_t hash)
{
	return factory.getReusableAsset(hash);
}

std::type_index ECSManager::getAssetTypeIndex(DefaultAsset defaultAsset)
{
	if (defaultAsset == DefaultAsset::CUBE)
		return std::type_index(typeid(MeshComponent));
	else
		assert(false);

	//INVALID
	return std::type_index(typeid(ECSManager));
}

std::type_index ECSManager::getAssetTypeIndex(const std::filesystem::path& filePath)
{
	if (filePath.extension() == ".obj")
		return std::type_index(typeid(MeshComponent));
	else if (filePath.extension() == ".png")
		return std::type_index(typeid(TextureComponent));
	else
		assert(false);

	//INVALID
	return std::type_index(typeid(ECSManager));
}

void ECSManager::removeComponentByRTTI(uint32 entityID, std::type_index componentType)
{
	if (componentType == std::type_index(typeid(TransformComponent)))
		removeComponent<TransformComponent>(entityID);
	else if (componentType == std::type_index(typeid(CameraComponent)))
		removeComponent<CameraComponent>(entityID);
	else if (componentType == std::type_index(typeid(MeshComponent)))
		removeComponent<MeshComponent>(entityID);
	else if (componentType == std::type_index(typeid(AxisAlignedBoxComponent)))
		removeComponent<AxisAlignedBoxComponent>(entityID);
	else if (componentType == std::type_index(typeid(SphereComponent)))
		removeComponent<SphereComponent>(entityID);
	else if (componentType == std::type_index(typeid(SelectionComponent)))
		removeComponent<SelectionComponent>(entityID);	
	else if (componentType == std::type_index(typeid(TextureComponent)))
		removeComponent<TextureComponent>(entityID);
	else
	{
		std::string msg{ "You are trying to remove component " };
		msg.append(componentType.name());
		msg.append(", but you have not added it to ");
		msg.append(__func__);
		msg.append(" at ");
		msg.append(__FILE__);

		DEBUG_LOG(msg);

		assert(false); // You need to add the component you are using above
	}
}