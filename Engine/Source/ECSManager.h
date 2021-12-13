#pragma once
#include "CoreMinimal.h"
#include <array>
#include <vector>
#include <typeindex>
#include <filesystem>
#include "Components/ComponentManager.h"
#include "Factory.h"
#include <algorithm>
enum class DefaultAsset : uint8;

class ECSManager
{
	friend class Engine;
	friend class WorldOutliner;
public:
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
	uint32 loadAsset(uint32 entityID, DefaultAsset defaultAsset);

	Factory::ReusableAsset getReusableAsset(std::size_t hash);

	template <typename T>
	class ComponentManager<T>* getComponentManager();

	int32 getNumberOfEntities() const;
	bool entityExists(int32 entityID) const;
	std::vector<std::pair<std::type_index, uint32>> getEntity(int32 entityID);
private:
	ECSManager();

	void removeComponentByRTTI(uint32 entityID, std::type_index componentType);
	class Factory& factory;
	template <typename... Ts>
	void swallow(Ts&&...);

	/** Reads as std::pair<isActive, std::vector<std::pair<type_index of component, componentID>>>
	* Iterating the entire array can be slow if entities are spread out, but that should not be necessary. 
	*/
	std::array<std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, core::MAX_ENTITIES>& entities;
	// Keeping record of available entity ids
	std::vector<uint32> &availableEntityIDs;

	std::type_index getAssetTypeIndex(DefaultAsset defaultAsset);
	std::type_index getAssetTypeIndex(const std::filesystem::path& filePath);
};
template<typename T>
inline ComponentManager<T>* ECSManager::getComponentManager()
{
	return factory.getComponentManager<T>();
}

template<typename... Types>
inline void ECSManager::addComponents(uint32 entityID)
{
	swallow(addComponent<Types>(entityID)...);
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
	auto& entt = entities[entityID].second;

	std::type_index index = std::type_index(typeid(T));

	if(entt.back().first == index)
		entt.pop_back();
	else
		for (uint64 i{ }; i < entt.size(); ++i)
		{

			//auto variable = entt.first;
			if (entt[i].first == index)
			{
				std::iter_swap(entities[entityID].second.begin() + i, entities[entityID].second.end()-1);
				entities[entityID].second.pop_back();
				break;
			}
		}


	return true;
}

template<typename ...Types>
inline void ECSManager::removeComponents(uint32 entityID)
{
	swallow(removeComponent<Types>(entityID)...);
}