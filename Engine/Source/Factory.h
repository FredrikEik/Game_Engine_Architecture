#pragma once
#include "CoreMinimal.h"
#include "Components/ComponentManager.h"
#include "Components/Components.h"
#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include "Systems/MeshSystem.h"
#include <iostream>

// There should only exist one factory, get it through the ECSManager
class Factory
{
	friend class ECSManager;
	Factory() : componentManagers{ (new std::unordered_map<std::type_index, ComponentManager<Component>*>) },
		reusableAssetComponents{ (*new std::unordered_map<std::size_t, ReusableAsset>) } {}
	~Factory();
public:
	

	// This is 2-3 times slower than creating components directly through the manager when creating many components.
	// If you need to create a lot of components, consider doing it through getManager directly.
	// When creating a few components, use this.
	template <typename T>
	uint32 createComponent(uint32 entityID, bool isReusable = false);

	// This is 10 times slower than removing directly through the manager when removing many components.
	// If you need to delete a lot of components, consider doing it through getManager directly.
	// For one component, this is faster
	template <typename T>
	void removeComponent(uint32 entityID);

	void removeComponent(uint32 entityID, std::type_index componentType, uint32 componentID);

	uint32 loadAsset(uint32 entityID, const std::filesystem::path& filePath);
	uint32 loadAsset(uint32 entityID, enum DefaultAsset defaultAsset);

	template <typename T>
	ComponentManager<T>& getManager();

private:
	uint32 loadMesh(const std::filesystem::path& filePath, uint32 entityID);

	bool assetExists(const std::filesystem::path& filePath);
	uint32 assignAsset(uint32 entityID, const std::filesystem::path& filePath);

	std::unordered_map<std::type_index, ComponentManager<Component>*>* componentManagers{};

	struct ReusableAsset
	{
		ReusableAsset(std::type_index type, uint32 compID, uint32 firstEntity)
			:componentType{type}, componentID{compID}, entitiesUsingAsset{firstEntity}
		{}
		std::type_index componentType;
		uint32 componentID;
		std::vector<uint32> entitiesUsingAsset;
	};

	/** Stores a pair based on the path
	* @param type_index to locate the component manager in componentManagers
	* @param uint32 is the component ID within the component manager. */
	std::unordered_map<std::size_t, ReusableAsset>& reusableAssetComponents;
};


// This halves the speed of creating components. 
// It is plenty fast enough for creating a few components
// TODO: find a way to use the direct route in situations where performance is critical, IF NEEDED.
template<typename T>
inline uint32 Factory::createComponent(uint32 entityID, bool isReusable)
{
	std::type_index classType = std::type_index(typeid(T));
	if (componentManagers->find(classType) == componentManagers->end())
	{
		ComponentManager<T> *manager = new ComponentManager<T>();
		uint32 componentID = manager->createComponent(entityID, isReusable);
		componentManagers->insert(std::pair<std::type_index, ComponentManager<Component>*>
			(classType, (ComponentManager<Component>*)manager));
		return componentID;
	}
	ComponentManager<T>* manager = (ComponentManager<T>*)componentManagers->at(classType);
	return manager->createComponent(entityID, isReusable);
}

template<typename T>
inline void Factory::removeComponent(uint32 entityID)
{
	std::type_index classType = std::type_index(typeid(T));
	assert(componentManagers->find(classType) != componentManagers->end());

	(*(ComponentManager<T>*)componentManagers->at(classType)).removeComponent(entityID);
}

inline uint32 Factory::loadAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	if (assetExists(filePath))
		return assignAsset(entityID, filePath);

	if (filePath.extension() == ".obj")
		return loadMesh(filePath, entityID);

	// Reading unsupported assets, the program should end.
	assert(false);
	return uint32();
}



inline bool Factory::assetExists(const std::filesystem::path& filePath)
{
	return reusableAssetComponents.find(std::filesystem::hash_value(filePath)) != reusableAssetComponents.end();
}

inline uint32 Factory::assignAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	ReusableAsset reusableAsset = 
		reusableAssetComponents.at(std::filesystem::hash_value(filePath));
	componentManagers->at(reusableAsset.componentType)->
		assignComponent(reusableAsset.componentID, entityID);
	reusableAsset.entitiesUsingAsset.push_back(entityID);
	//TODO: Remove sort if position is not important
	std::sort(reusableAsset.entitiesUsingAsset.begin(), reusableAsset.entitiesUsingAsset.end());

	return reusableAsset.componentID;
}


template<typename T>
inline ComponentManager<T>& Factory::getManager()
{
	return (*(ComponentManager<T>*)componentManagers->at(std::type_index(typeid(T))));
}

inline uint32 Factory::loadMesh(const std::filesystem::path& filePath, uint32 entityID)
{
	uint32 componentID = createComponent<MeshComponent>(entityID, true);
	MeshSystem::loadMesh(filePath, getManager<MeshComponent>().getComponent(entityID));

	ReusableAsset reusableAsset(std::type_index(typeid(MeshComponent)), componentID, entityID);

	reusableAssetComponents.insert(std::pair< std::size_t, ReusableAsset>
				(std::filesystem::hash_value(filePath), reusableAsset));

	return componentID;
}
