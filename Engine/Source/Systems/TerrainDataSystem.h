#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <string>
#include <filesystem>
#include "../Components/Components.h"


class TerrainDataSystem final : public BaseSystem
{
public:
	static std::tuple<float, float, float, float, float, float> generateLAS(std::vector<Vertex>& data, MeshComponent* gridMesh, MeshComponent& terrainMesh, class ECSManager* ECS);
	static std::vector<Vertex> readLAS(const std::filesystem::path& filePath);
	//static void drawGrid(class Shader* shader, const std::string& uniformName, class ECSManager* ECS, GridComponent* gridComp);
	//static void drawTerrain(class Shader* shader, const std::string& uniformName, class ECSManager* ECS, MeshComponent& terrainMesh);

	static void makeGrid(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, float resolution, const std::vector<Vertex>& Vertices,
		uint32 LASEntity, uint32 GridEntity, uint32 ContourEntity, ECSManager* ECS);

	static glm::vec3 getNormalByIndex(uint32 terrainEntity, ECSManager* ECS, int index);
};