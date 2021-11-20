#pragma once
#include "../CoreMinimal.h"
#include <vector>
#include <typeindex>
#include "../JSON/json.hpp"

class Save
{
public:
	static void saveEntities(const std::array<
		std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, 
		core::MAX_ENTITIES>& entities, 
		uint32 startIndex, class ECSManager* ECS);
private:
	static void addComponentToJson(std::type_index type, uint32 entityID, nlohmann::json& OUTjson,
			class ECSManager* ECS);
	//static void WriteComponentValues();
};