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
	static float getHeight(uint32 entity, uint32 terrainEntity, class ECSManager* ECS);
	static float getHeight(const struct TransformComponent& entityTransform, const struct MeshComponent& terrainMesh,
		 int32 &OUTIndex);

	static float getHeightFast(const struct TransformComponent& entityTransform, const struct MeshComponent& terrainMesh,
		const int32& index);

	static glm::vec3 getNormal(const struct TransformComponent& entityTransform,
		const struct MeshComponent& terrainMesh, const int32& Index);

private:
	static void calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize, 
		float stepDistanceX, float stepDistanceY, std::vector<glm::vec3>& OUTNormals);

	static bool findTriangle(uint64 index, const glm::vec3& position, const struct MeshComponent& terrainMesh,
		glm::vec3& outBaryCoord, glm::vec3& outP, glm::vec3& outQ, glm::vec3& outR);

	static glm::vec3 baryCentricCoordinates(const glm::vec2& position, const glm::vec2& p1,
		const glm::vec2& p2, const glm::vec2& p3);

	static float terrainResolution;
	static float errorMargin;

};

inline float TerrainSystem::terrainResolution = 10;
inline float TerrainSystem::errorMargin = 0.98f;