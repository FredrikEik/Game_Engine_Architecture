#include "TerrainSystem.h"
#include "../ECSManager.h"
#include "MeshSystem.h"
#include <fstream>
void TerrainSystem::generateRegularGrid(uint32 entity, ECSManager* ECS)
{
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
	std::vector<Vertex>& vertices = (*new std::vector<Vertex>());
	std::vector<glm::vec3>& positions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& normals = (*new std::vector<glm::vec3>());
	std::vector<GLuint>& indices = (*new std::vector<GLuint>());
	int64 rows{ 300};
	int64 columns{ 300 };

	vertices.reserve(rows * columns);
	positions.reserve(rows * columns);

	for (uint64 i{}; i < rows; ++i)
	{
		for (uint64 j{}; j < columns; ++j)
		{
			positions.push_back(glm::vec3(i, 0, j));
			//positions.push_back(glm::vec3(i, std::rand()%10, j));
		}
	}
	int rowsLooped{ 1 };
	for (uint64 i{}; i < positions.size()-columns-1; i++)
	{
		// Don't push back indices outside the edge
		if (((i+1) % (columns*rowsLooped)) == 0 )
		{
			rowsLooped++;
			continue;
		}
		//indices.push_back(i);
		//indices.push_back(i+1);
		//indices.push_back(i+columns);

		//indices.push_back(i+1);
		//indices.push_back(i+1+columns);
		//indices.push_back(i+columns);		
		
		indices.push_back(i);
		indices.push_back(i+1+columns);
		indices.push_back(i+columns);

		indices.push_back(i);
		indices.push_back(i+1);
		indices.push_back(i+columns+1);
	}

	calculateNormals(positions, columns, 1, 1, normals);

	rowsLooped = 1;
	//int32 columnsLooped = 0;
	for (int32 i{}; i<positions.size(); ++i)
	{
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			//columnsLooped = 0;
			rowsLooped++;
		}
		mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1-(rowsLooped % 2)), (i%2))));
		//mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1-(rowsLooped % 2)), (columnsLooped %2))));
		//++columnsLooped;
		//mesh->m_vertices.push_back(Vertex(it, glm::vec3(0, 1, 0), glm::vec2(0, 0)));
	}
	mesh->m_indices = indices;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;
}

void TerrainSystem::generateGridFromLAS(uint32 entity, std::filesystem::path path, ECSManager* ECS)
{
	std::ifstream file(path);
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
	assert(file);
	assert(mesh);
	std::vector<glm::vec3>& lasPositions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& positions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& normals = (*new std::vector<glm::vec3>());
	std::vector<GLuint>& indices = (*new std::vector<GLuint>());

	float terrainResolution{ 10 };

	float x, y, z;
	glm::vec3 min{};
	glm::vec3 max{};
	int32 points{};

	file >> points; // The first line does not contain position info
	lasPositions.reserve(points);

	bool initialized{ false };

	while (file)
	{
		file >> x;
		file >> z;
		file >> y;

		if (!initialized)
		{
			// Making sure the min and max values are valid
			min = glm::vec3(x, y, z);
			max = glm::vec3(x, y, z);
			initialized = true;
		}
		// Calculating the lowest value in the file
		if (x < min.x) min.x = (x);
		if (y < min.y) min.y = (y);
		if (z < min.z) min.z = (z);

		// Calculating the highest value in the file
		if (x > max.x) max.x = (x);
		if (y > max.y) max.y = (y);
		if (z > max.z) max.z = (z);

		lasPositions.push_back(glm::vec3(x, y, z));
	}
	/* @brief A map that will contain the height values, sorted based on the position they will have
	 @param int position in array
	 @param float total height 
	 @param int number of that height*/
	std::map<int, std::pair<float, int>> averageHeightPerSquare;

	// Lambda that adds the positions to the averageHeightPerSquare container
	auto addPositionToAvgHeight =
		[terrainResolution, min](std::map<int, std::pair<float, int>>& container, const glm::vec3& position)
	{
		float positionRow = (position.x - min.x) / terrainResolution;
		float positionColumn = (position.z - min.z) / terrainResolution;
		long positionInArray = std::lroundf(positionColumn * positionRow);
		if (container.find(positionInArray) != container.end())
		{
			container.at(positionInArray).first += position.y;
			container.at(positionInArray).second++;
		}
		else
		{
			container.insert(std::pair<int, std::pair<float, int>>(positionInArray, std::pair<float, int>(position.y, 1)));
		}
	};

	// Looping through all positions and gets the average height per vertex
	for (const auto& it : lasPositions)
	{
		addPositionToAvgHeight(averageHeightPerSquare, it);
	}

	glm::vec3 center = (min + max) / 2.f;
	
	uint64 columns = std::sqrt(averageHeightPerSquare.size());
	normals.reserve(averageHeightPerSquare.size());
	indices.reserve(averageHeightPerSquare.size());
	positions.reserve(averageHeightPerSquare.size());

	// Using the for each loop as a regular nested for loop to ensure there are no gaps
	uint64 columnsLooped = 0;
	uint64 rowsLooped = 0;
	for (auto& it : averageHeightPerSquare)
	{
		if (columnsLooped >= columns)
		{
			columnsLooped = 0;
			rowsLooped++;
		}
		it.second.first /= (float)it.second.second;
		x = rowsLooped;
		x *= terrainResolution;
		z = columnsLooped;
		z *= terrainResolution;
		positions.push_back(glm::vec3(x, it.second.first - center.y, z));
		++columnsLooped;
	}

	rowsLooped = { 1 };
	for (uint64 i{}; i < positions.size() - columns - 1; i++)
	{
		// Don't push back indices outside the edge
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			rowsLooped++;
			continue;
		}
		indices.push_back(i);
		indices.push_back(i + 1 + columns);
		indices.push_back(i + columns);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + columns + 1);
	}

	calculateNormals(positions, columns, terrainResolution, terrainResolution, normals);

	rowsLooped = 1;
	for (int32 i{}; i < positions.size(); ++i)
	{
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			rowsLooped++;
		}
		mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1 - (rowsLooped % 2)), (i % 2))));
	}
	mesh->m_indices = indices;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;

}

void TerrainSystem::calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize,
	float stepDistanceX, float stepDistanceY, std::vector<glm::vec3>& OUTNormals)
{
	uint32 rowsLooped{ 1 };

	float yLeft, yRight, yUp, yDown, yUpRight, yDownLeft;

	auto getHeight = [rowSize](const std::vector<glm::vec3>& positions, uint32 i, uint32& rowsLooped) -> float
	{
		if (i < (positions.size() - 1) && i > 0)
		{
			if (((i) % (rowSize * rowsLooped)) == 0)
			{
				return 0.f;
				//rowsLooped++;
			}
			else
				return positions[i].y;
		}
		else
			return 0;
	};

	for (uint32 i{}; i < positions.size() - 1; i++)
	{
		yLeft = getHeight(positions, i - 1, rowsLooped);
		yRight = getHeight(positions, i + 1, rowsLooped);
		yUp = getHeight(positions, i + rowSize, rowsLooped);
		yDown = getHeight(positions, i - rowSize, rowsLooped);
		yUpRight = getHeight(positions, i + rowSize + 1, rowsLooped);
		yDownLeft = getHeight(positions, i - rowSize - 1, rowsLooped);

		// Instead of doing cross product of 6 triangles
		// This is the same
		glm::vec3 normal ((2 * (yLeft - yRight) - yUpRight + yDownLeft + yUp - yDown ) / stepDistanceX,
			6,
			(2 * (yDown - yUp) + yUpRight + yDownLeft - yUp - yLeft) / stepDistanceY
		);
		normal = glm::normalize(normal);
		OUTNormals.push_back(normal);

		if (((i + 1) % (rowSize * rowsLooped)) == 0)
		{
			rowsLooped++;
		}

	}
}
