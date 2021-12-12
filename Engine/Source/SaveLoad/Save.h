#pragma once
#include "../CoreMinimal.h"
#include <vector>
#include <typeindex>
#include "glm/glm.hpp"
#include "../JSON/json.hpp"

class Save
{
public:
	static void saveEntities(const std::array<
		std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, 
		core::MAX_ENTITIES>& entities, 
		uint32 startIndex, class ECSManager* ECS);
	static void saveEntityPrefab(uint32 entityID, class ECSManager* ECS);

	static std::string getDefaultAbsolutePath();

	static nlohmann::json vec3JSON(const glm::vec3& v);
	static nlohmann::json vec4JSON(const glm::vec4& v);
private:
	static void addComponentToJson(std::type_index type, uint32 entityID, nlohmann::json& OUTjson,
			class ECSManager* ECS);

	static std::string defaultSaveLocation;
	static std::string defaultSaveName;
	//static void WriteComponentValues();
};