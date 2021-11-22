#include "TerrainSystem.h"
#include "../ECSManager.h"
#include "MeshSystem.h"
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
			//positions.push_back(glm::vec3(i, 0, j));
			positions.push_back(glm::vec3(i, std::rand()%10, j));
		}
	}
	int rowsLooped{ 1 };
	for (uint64 i{}; i < positions.size()-columns-1; i++)
	{
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

	calculateNormals(positions, columns, normals);

	for (int32 i{}; i<positions.size(); ++i)
	{
		mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2(0, 0)));
		//mesh->m_vertices.push_back(Vertex(it, glm::vec3(0, 1, 0), glm::vec2(0, 0)));
	}
	mesh->m_indices = indices;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;
}

void TerrainSystem::calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize,
	std::vector<glm::vec3>& OUTNormals)
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

		glm::vec3 normal ((2 * (yLeft - yRight) - yUpRight + yDownLeft + yUp - yDown),
			6,
			(2 * (yDown - yUp) + yUpRight + yDownLeft - yUp - yLeft));
		normal = glm::normalize(normal);
		OUTNormals.push_back(normal);

		if (((i + 1) % (rowSize * rowsLooped)) == 0)
		{
			rowsLooped++;
		}
	}
}
