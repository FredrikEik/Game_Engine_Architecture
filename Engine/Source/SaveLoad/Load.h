#pragma once
#include "../CoreMinimal.h"
#include <filesystem>
#include "../JSON/json.hpp"


/// <summary>
/// Used for any and all loading. Can load a single component, single entity, or an entire score of entities 
/// </summary>
class Load
{
public:
	static void loadEntities(const std::filesystem::path& filePath, class ECSManager* ECS);
	static uint32 loadEntity(uint32 entityID, const std::filesystem::path& filePath, class ECSManager* ECS);
	static void loadComponent(const std::filesystem::path& filePath, class Component* component);

private:
	static class Component* createComponent(std::string componentName, nlohmann::json jsonValue,
		uint32 entityID, class ECSManager* ECS);
};