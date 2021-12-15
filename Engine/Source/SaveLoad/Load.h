#pragma once
#include "../CoreMinimal.h"
#include <filesystem>
#include "../JSON/json.hpp"
class Load
{
public:
	static void loadEntities(const std::filesystem::path& filePath, class ECSManager* ECS);
	static void loadComponent(const std::filesystem::path& filePath, class Component* component);

private:
	static class Component* createComponent(std::string componentName, nlohmann::json jsonValue,
		uint32 entityID, class ECSManager* ECS);
};