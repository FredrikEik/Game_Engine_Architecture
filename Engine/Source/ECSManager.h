#pragma once
#include "CoreMinimal.h"
#include <array>
#include <vector>
#include <typeindex>
#include <filesystem>
#include "Components/ComponentManager.h"
#include "Factory.h"

class ECSManager
{
public:
	ECSManager();
	~ECSManager();

	template<typename T>
	uint32 addComponent(uint32 entityID);
	template<typename ... Types>
	void addComponents(uint32 entityID);

	template<typename T>
	bool removeComponent(uint32 entityID);
	template<typename ... Types>
	void removeComponents(uint32 entityID);

	void destroyEntity(uint32 entityID);

	uint32 newEntity();

	void printEntity(uint32 entityID);

	uint32 loadAsset(uint32 entityID, const std::filesystem::path& filePath);
	uint32 loadAsset(uint32 entityID, enum DefaultAsset defaultAsset);

	template <typename T>
	class ComponentManager<T>& getManager();
private:
	class Factory& factory;
	template <typename... Ts>
	void swallow(Ts&&...);

	/** Reads as std::pair<isActive, std::pair<type_index of component, componentID>>
	* Iterating the entire array can be slow if entities are spread out, but that should not be necessary. 
	*/
	std::array<std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, core::MAX_ENTITIES>& entities;
	// Keeping record of available entity ids
	std::vector<uint32> &availableEntityIDs;
};

template<typename... Types>
inline void ECSManager::addComponents(uint32 entityID)
{
	swallow(addComponent<Types>(entityID)...);
}


template<typename T>
inline ComponentManager<T>& ECSManager::getManager()
{
	return factory.getManager<T>();
}

// Don't mind this. Just unpacking variadic template arguments :))))
template <typename... Ts>
inline void ECSManager::swallow(Ts&&...) 
{
}

template<typename T>
inline uint32 ECSManager::addComponent(uint32 entityID)
{
	uint32 componentID{ factory.createComponent<T>(entityID) };
	std::pair<std::type_index, uint32> componentLocation(std::type_index(typeid(T)), componentID);

	entities.at(entityID).second.push_back(componentLocation); // THIS FUCKS UP

	return componentID;
}

template<typename T>
inline bool ECSManager::removeComponent(uint32 entityID)
{
	factory.removeComponent<T>(entityID);

	return true;
}

template<typename ...Types>
inline void ECSManager::removeComponents(uint32 entityID)
{
	swallow(removeComponent<Types>(entityID)...);
}