#pragma once
#include "BaseSystem.h"
#include <vector>
#include "glm/glm.hpp"
class TerrainSystem : public BaseSystem
{
public:
	static void generateRegularGrid(uint32 entity, class ECSManager* ECS);
private:
	static void calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize, std::vector<glm::vec3>& OUTNormals);
};

