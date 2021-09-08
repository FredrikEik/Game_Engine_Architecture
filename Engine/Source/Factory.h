#pragma once
#include "CoreMinimal.h"
#include "Components/ComponentManager.h"
#include "Components/Components.h"
#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include "Systems/MeshSystem.h"


//TODO: std::filesystem::path is not hashable. Swap out for a hashable key. E.G std::filesystem::hash_value
class Factory
{
public:
	//friend class ComponentManager;
	
	Factory(): componentManagers{(*new std::unordered_map<std::type_index, ComponentManager<Component>*>)},
		reusableAssetComponents{(*new std::unordered_map<std::filesystem::path, std::pair<std::type_index, uint32>>)} {}
	~Factory();

	// This is 2-3 times slower than creating components directly through the manager when creating many components.
	// If you need to create a lot of components, consider doing it through getManager directly.
	// When creating a few components, use this.
	template <typename T>
	uint32 createComponent(uint32 entityID);

	// This is 10 times slower than removing directly through the manager when removing many components.
	// If you need to delete a lot of components, consider doing it through getManager directly.
	// For one component, this is faster
	template <typename T>
	void removeComponent(uint32 entityID);


	template <typename T>
	uint32 loadAsset(uint32 entityID, const std::filesystem::path& filePath);

	template <typename T>
	ComponentManager<T>& getManager();

private:
	uint32 loadMesh(const std::filesystem::path& filePath, uint32 entityID);

	bool assetExists(const std::filesystem::path& filePath);
	uint32 assignAsset(uint32 entityID, const std::filesystem::path& filePath);

	std::unordered_map<std::type_index, ComponentManager<Component>*>& componentManagers;
	/** Stores a pair based on the path
	* @param type_index to locate the component manager in componentManagers
	* @param uint32 is the component ID within the component manager. */
	std::unordered_map<std::filesystem::path, std::pair<std::type_index, uint32>>& reusableAssetComponents;
};

Factory::~Factory()
{
	delete& componentManagers;
	delete& reusableAssetComponents;
}



// This halves the speed of creating components. 
// It is plenty fast enough for creating a few components
// TODO: find a way to use the direct route in situations where performance is critical, IF NEEDED.
template<typename T>
inline uint32 Factory::createComponent(uint32 entityID)
{
	std::type_index classType = std::type_index(typeid(T));
	if (componentManagers.find(classType) == componentManagers.end())
	{
		ComponentManager<T> *manager = new ComponentManager<T>();
		uint32 componentID = manager->createComponent(entityID);
		componentManagers.insert(std::pair<std::type_index, ComponentManager<Component>*>
			(classType, (ComponentManager<Component>*)manager));
		return componentID;
	}
	ComponentManager<T>* manager = (ComponentManager<T>*)componentManagers.at(classType);
	return manager->createComponent(entityID);
}

template<typename T>
inline void Factory::removeComponent(uint32 entityID)
{
	std::type_index classType = std::type_index(typeid(T));
	assert(componentManagers.find(classType) != componentManagers.end());

	(*(ComponentManager<T>*)componentManagers.at(classType)).removeComponent(entityID);
}

template <typename T>
inline uint32 Factory::loadAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	if (assetExists(filePath))
		return assignAsset(entityID, filePath);

	if (filePath.extension() == ".obj")
		return loadMesh(filePath);

	// Reading unsupported assets, the program should end.
	assert(true);
	return uint32();
}

inline bool Factory::assetExists(const std::filesystem::path& filePath)
{
	return reusableAssetComponents.find(filePath) != reusableAssetComponents.end();
}

inline uint32 Factory::assignAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	//std::pair<std::type_index, uint32> reusableLocator = reusableAssetComponents.at(filePath);
	//componentManagers.at(reusableLocator.first)->assignComponent(reusableLocator.second, entityID);
	//return reusableLocator.second;
	return 0;
}


template<typename T>
inline ComponentManager<T>& Factory::getManager()
{
	return (*(ComponentManager<T>*)componentManagers.at(std::type_index(typeid(T))));
}

inline uint32 Factory::loadMesh(const std::filesystem::path& filePath, uint32 entityID)
{
	uint32 meshComponent = createComponent<MeshComponent>(entityID);
	MeshSystem::loadMesh(filePath, getManager<MeshComponent>().getComponent(entityID));
	return meshComponent;
}
