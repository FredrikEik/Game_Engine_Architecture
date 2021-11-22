#pragma once
#include "BaseSystem.h"
#include <vector>
#include "glm/glm.hpp"
#include <filesystem>
class TerrainSystem : public BaseSystem
{
public:
	static void generateRegularGrid(uint32 entity, class ECSManager* ECS);
	static void generateGridFromLAS(uint32 entity, std::filesystem::path path,class ECSManager* ECS);
private:
	static void calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize, 
		float stepDistanceX, float stepDistanceY, std::vector<glm::vec3>& OUTNormals);
};

