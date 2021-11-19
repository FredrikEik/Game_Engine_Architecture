#include "Save.h"
#include "../JSON/json.hpp"
#include "../ECSManager.h"
#include "../Components/Components.h"
#include <fstream>
#include <filesystem>
void Save::SaveEntities(const std::array<
	std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>,
	core::MAX_ENTITIES>& entities, 
	uint32 startIndex, class ECSManager* ECS)
{
	nlohmann::json json = nlohmann::json::array();
	for (uint32 i{ startIndex }; i < entities.size(); ++i)
	{
		if (!entities[i].first) // Entity is not in use
			continue;

		for (const auto& it : entities[i].second)
		{
			if (it.first == std::type_index(typeid(TransformComponent)))
			{
				TransformComponent* comp = ECS->getComponentManager<TransformComponent>()->getComponentChecked(i);
				if (!comp)
					continue;

				std::string test(it.first.name());
				//json[i][test][j][k] = comp->transform[j][k];
				//json[i] = 50;
				nlohmann::json json2 = { {test, 50} };
				for (int j{}; j<4; ++j)
					for (int k{}; k < 4; ++k)
					{
						assert(false); // Keep working here
						 // Make this work. Should just save values and read them back into the array no worries
						//json2[j][k] = comp->transform[j][k];
					}
				json.push_back(json2);
			}
		}
	}

	std::filesystem::create_directory("../scenes");
	std::ofstream file("../scenes/test.json");
	assert(file);
	file << std::setw(4) << json << std::endl;
	file.close();
}
