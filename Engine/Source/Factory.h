#pragma once
#include "CoreMinimal.h"
#include "Components/ComponentManager.h"
#include "Components/Components.h"
#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include "Systems/MeshSystem.h"
#include "Systems/TextureSystem.h"
#include <iostream>
#include <algorithm>
#include "FileSystemHelpers.h"
enum class DefaultAsset : uint8;


// There should only exist one factory, get it through the ECSManager
/// <summary>
/// Object factory. It manages all the ComponentManagers and reusable assets, and is managed by ECSManager
/// Internal class, never instantiate outside of ECSManager.
/// </summary>
class Factory
{
	friend class ECSManager;
	friend class Save;
	Factory() : componentManagers{ (new std::unordered_map<std::type_index, ComponentManager<Component>*>) },
		reusableAssetComponents{ (*new std::unordered_map<std::size_t, ReusableAsset>) } {componentManagers->reserve(50); }
	~Factory();

public:
	/// <summary>
	/// Keeps the Run Time Type Index(RTTI) and component ID of a reusable asset
	/// </summary>
	struct ReusableAsset
	{
		ReusableAsset(std::type_index type, uint32 compID)
			:componentType{ type }, componentIDs{ compID }/*, entitiesUsingAsset{ firstEntity }*/
		{}
		std::type_index componentType;
		std::vector<uint32> componentIDs;
		//std::vector<uint32> entitiesUsingAsset;
	};


	/// <summary>
	/// Gets the component manager of the component type
	/// </summary>
	/// <returns>The component manager of the component type</returns>
	template <typename T>
	ComponentManager<T>* getComponentManager();

	/// <summary>
	/// Gets the getReusableAsset of hash
	/// </summary>
	/// <returns>The reusable asset</returns>
	struct ReusableAsset getReusableAsset(std::size_t hash);

private:
	// This is 2-3 times slower than creating components directly through the manager when creating many components.
	// If you need to create a lot of components, consider doing it through getManager directly.
	// When creating a few components, use this.


	/// <summary>
	/// Creates a component of type T.
	/// This is 2-3 times slower than creating components directly through the manager when creating many components.
	/// If you need to create a lot of components, consider doing it through getManager directly.
	/// When creating a few components, use this.
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="isReusable">if set to <c>true</c> [is reusable].</param>
	/// <returns>The component ID of the new component</returns>
	template <typename T>
	uint32 createComponent(uint32 entityID, bool isReusable = false);

	// This is 10 times slower than removing directly through the manager when removing many components.
	// If you need to delete a lot of components, consider doing it through getManager directly.
	// For one component, this is faster

	/// <summary>
	/// Removes the entity's component of type T. 
	/// This is 10 times slower than removing directly through the manager when removing many components.
	/// If you need to delete a lot of components, consider doing it through getManager directly.
	/// For one component, this is faster
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	template <typename T>
	void removeComponent(uint32 entityID);


	/// <summary>
	/// Loads an asset based on the file extension. If the file has been loaded before, the component is reused
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="filePath">The file path.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 loadAsset(uint32 entityID, const std::filesystem::path& filePath);
	uint32 loadAsset(uint32 entityID, DefaultAsset defaultAsset);


	/// <summary>
	/// Loads the mesh. If the file has been loaded before, the component is reused
	/// </summary>
	/// <param name="filePath">The file path.</param>
	/// <param name="entityID">The entity identifier.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 loadMesh(const std::filesystem::path& filePath, uint32 entityID);


	/// <summary>
	/// Assigns a reusable mesh instead of creating a new. 
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="filePath">The file path.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 assignMesh(uint32 entityID, const std::filesystem::path& filePath);


	/// <summary>
	/// Loads the image. If the file has been loaded before, the component is reused
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="filePath">The file path.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 loadImage(uint32 entityID, const std::filesystem::path& filePath);

	/// <summary>
	/// Assigns a reusable image instead of creating a new. 
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="filePath">The file path.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 assignImage(uint32 entityID, const std::filesystem::path& filePath);

	/// <summary>
	/// Checks if an asset has been loaded and exists in memory
	/// </summary>
	/// <param name="filePath">The file path.</param>
	/// <returns>True if the file has been loaded and can be reused</returns>
	bool assetExists(const std::filesystem::path& filePath);

	/// <summary>
	/// Assigns a reusable asset.
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="filePath">The file path.</param>
	/// <returns>The component ID of the new component</returns>
	uint32 assignAsset(uint32 entityID, const std::filesystem::path& filePath);

	/// <summary>
	/// Removes an invalid reusable asset in case it no longer exists. E.g if all entities that held it is destroyed.
	/// In case the asset is valid it returns a pointer to it instead
	/// </summary>
	/// <param name="filePath">The file path.</param>
	/// <returns>A pointer to a valid reusable asset in case it is not invalid. nullptr otherwise </returns>
	template<typename T>
	T* removeInvalidReusableAsset(const std::filesystem::path& filePath);

	std::unordered_map<std::type_index, ComponentManager<Component>*>* componentManagers{};


	/** Stores a pair based on the path
	* @param type_index to locate the component manager in componentManagers
	* @param uint32 is the component ID within the component manager. */
	std::unordered_map<std::size_t, ReusableAsset>& reusableAssetComponents;
};


// This halves the speed of creating components. 
// It is plenty fast enough for creating a few components
template<typename T>
inline uint32 Factory::createComponent(uint32 entityID, bool isReusable)
{
	std::type_index classType = std::type_index(typeid(T));

	// If no component manager for this type exist, create one
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
	ComponentManager<T>* manager = (ComponentManager<T>*)componentManagers->at(classType);

	manager->removeComponent(entityID);
}


inline uint32 Factory::loadAsset(uint32 entityID, const std::filesystem::path& filePath)
{
	if (assetExists(filePath))
		return assignAsset(entityID, filePath);

	if (filePath.extension() == ".obj")
		return loadMesh(filePath, entityID);
	else if (filePath.extension() == ".png" || filePath.extension() == ".jpg")
		return loadImage(entityID, filePath);

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
	ReusableAsset &reusableAsset = 
		reusableAssetComponents.at(std::filesystem::hash_value(filePath));

	// "Manually" assign reusable assets based on asset type. 
	if (reusableAsset.componentType == std::type_index(typeid(MeshComponent)))
		return assignMesh(entityID, filePath);
	else if (reusableAsset.componentType == std::type_index(typeid(TextureComponent)))
		return assignImage(entityID, filePath);
	else
		assert(false); // You have to add new components to the if above. Soowry

	return core::MAX_ENTITIES + 1;
}


template<typename T>
inline ComponentManager<T>* Factory::getComponentManager()
{
	if (componentManagers->find(std::type_index(typeid(T))) == componentManagers->end())
		return nullptr;

	return ((ComponentManager<T>*)componentManagers->at(std::type_index(typeid(T))));
}


inline uint32 Factory::loadMesh(const std::filesystem::path& filePath, uint32 entityID)
{
	uint32 componentID = createComponent<MeshComponent>(entityID, true);
	MeshComponent& component = getComponentManager<MeshComponent>()->getComponent(entityID);
	MeshSystem::loadMesh(filePath, component);

	ReusableAsset reusableAsset(std::type_index(typeid(MeshComponent)), componentID);

	std::size_t hash{ std::filesystem::hash_value(filePath) };
	component.hash = hash;
	component.path = FileSystemHelpers::extractRelativePath(filePath).string();
	
	reusableAssetComponents.insert(std::pair<std::size_t, ReusableAsset>
				(hash, reusableAsset));

	return componentID;
}


inline uint32 Factory::assignMesh(uint32 entityID, const std::filesystem::path& filePath)
{
	ReusableAsset& reusableAsset =
		reusableAssetComponents.at(std::filesystem::hash_value(filePath));

	ComponentManager<MeshComponent>* manager = getComponentManager<MeshComponent>();

	MeshComponent* oldComponent{}; 

	if (!removeInvalidReusableAsset<MeshComponent>(filePath))
		return loadAsset(entityID, filePath);

	uint32 componentID = createComponent<MeshComponent>(entityID, true);
	MeshComponent& newComponent = manager->getComponent(entityID);
	oldComponent = removeInvalidReusableAsset<MeshComponent>(filePath);

	assert(oldComponent);

	MeshSystem::copyMesh(*oldComponent, newComponent);
	reusableAsset.componentIDs.push_back(componentID); // Add the componentID to the reusable asset

	return componentID;
}

inline uint32 Factory::loadImage(uint32 entityID, const std::filesystem::path& filePath)
{
	uint32 componentID = createComponent<TextureComponent>(entityID, true);
	TextureComponent* component = getComponentManager<TextureComponent>()->getComponentChecked(entityID);
	TextureSystem::loadImage(entityID, filePath, component);

	ReusableAsset reusableAsset(std::type_index(typeid(TextureComponent)), componentID);

	std::size_t hash{ std::filesystem::hash_value(filePath) };
	component->hash = hash;
	component->path = FileSystemHelpers::extractRelativePath(filePath).string();

	reusableAssetComponents.insert(std::pair<std::size_t, ReusableAsset>
			(hash, reusableAsset));

	return componentID;
}

inline uint32 Factory::assignImage(uint32 entityID, const std::filesystem::path& filePath)
{

	if (!removeInvalidReusableAsset<TextureComponent>(filePath))
		return loadAsset(entityID, filePath);

	ReusableAsset& reusableAsset =
		reusableAssetComponents.at(std::filesystem::hash_value(filePath));

	ComponentManager<TextureComponent>* manager = getComponentManager<TextureComponent>();

	uint32 componentID = createComponent<TextureComponent>(entityID, true);
	TextureComponent* newComponent = manager->getComponentChecked(entityID);
	TextureComponent* oldComponent{ removeInvalidReusableAsset<TextureComponent>(filePath) };

	assert(oldComponent);

	newComponent->textureID = oldComponent->textureID;
	newComponent->hash = oldComponent->hash;
	newComponent->path = oldComponent->path;
	newComponent->wrapMode = oldComponent->wrapMode;
	reusableAsset.componentIDs.push_back(componentID); // Add the componentID to the reusable asset

	return componentID;
}

template<typename T>
inline T* Factory::removeInvalidReusableAsset(const std::filesystem::path& filePath)
{
	ReusableAsset& reusableAsset =
		reusableAssetComponents.at(std::filesystem::hash_value(filePath));

	ComponentManager<T>* manager = getComponentManager<T>();

	T* component{};

	for (uint32 i{}; i < reusableAsset.componentIDs.size(); ++i)
	{
		component = manager->getComponentFromID(reusableAsset.componentIDs.at(i));
		if (!component)
		{
			reusableAsset.componentIDs.erase(reusableAsset.componentIDs.begin() + i);
			--i;
		}
		else
			return component;
	}

	reusableAssetComponents.erase(std::filesystem::hash_value(filePath));

	return nullptr;
}
