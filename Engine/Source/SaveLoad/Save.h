#pragma once
#include "../CoreMinimal.h"
#include <vector>
#include <typeindex>
class Save
{
public:
	static void SaveEntities(const std::array<
		std::pair<bool, std::vector<std::pair<std::type_index, uint32>>>, 
		core::MAX_ENTITIES>& entities, 
		uint32 startIndex, class ECSManager* ECS);
private:

	//static void WriteComponentValues();
};