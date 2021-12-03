#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "TerrainDataSystem.h"
#include "glad/glad.h"
#include "../Assets/DefaultAssets.h"
#include "../Components/ComponentManager.h"
#include "../ECSManager.h"
#include "../Shader.h"
#include "CameraSystem.h"
#include "../Input/Input.h"
#include <utility> // std::pair
#include <tuple>
#include "../Line.h"
#include <math.h>
#include "../Systems/TransformSystem.h"
#include <glm/glm.hpp>
#include <execution>
#include <mutex>



std::tuple<float, float, float, float, float, float> getMinMax(const std::vector<Vertex>& Vertices)
{
	if (Vertices.size() <= 0)
		return std::make_tuple(0.f, 0.f,0.f,0.f, 0.f, 0.f);

	// init points to a value from the vertices
	float xMin = Vertices[0].m_xyz[0];
	float xMax = Vertices[0].m_xyz[0];

	float yMin = Vertices[0].m_xyz[1];
	float yMax = Vertices[0].m_xyz[1];

	float zMin = Vertices[0].m_xyz[2];
	float zMax = Vertices[0].m_xyz[2];

	for (const Vertex& vert : Vertices)
	{
		if (xMin > vert.m_xyz[0])
			xMin = vert.m_xyz[0];
		if (xMax < vert.m_xyz[0])
			xMax = vert.m_xyz[0];

		if (yMin > vert.m_xyz[1])
			yMin = vert.m_xyz[1];
		if (yMax < vert.m_xyz[1])
			yMax = vert.m_xyz[1];

		if (zMin > vert.m_xyz[2])
			zMin = vert.m_xyz[2];
		if (zMax < vert.m_xyz[2])
			zMax = vert.m_xyz[2];

	}

	return std::make_tuple(xMin, xMax, yMin, yMax, zMin, zMax);
}

// helper functions
float lerp(float v0, float v1, float t)
{
	return v0 + t * (v1 - v0);
}

glm::vec3 lerp(const glm::vec3& A, const glm::vec3& B, float t)
{
	return A * t + B * (1.f - t);
}

std::pair<glm::vec3,glm::vec3> getStartEnd(const glm::vec3& a, const glm::vec3& b)
{
	auto startValue = a.y < b.y ? a : b;
	auto endValue = a.y > b.y ? a : b;

	return std::make_pair(startValue, endValue);
}

bool isBetween(float start, float end, float x)
{
	if (x > start && x < end)
		return true; // is within
	else
		return false;
}

bool calculatePointForIsoContour(glm::vec3 A, glm::vec3 B, float isoValue, glm::vec3& outPoint)
{
	// to do the isbetween compare correctly by sorting height through get start end.
	auto edge = getStartEnd(A, B);
	// check isov	alue is between the height of the 2 edges
	bool b = isBetween(std::get<0>(edge).y, std::get<1>(edge).y, isoValue);
	if (b)
	{
		float alpha = (std::get<1>(edge).y - std::get<0>(edge).y) / (100); // 100 should be yMax not magic number
		std::get<0>(edge).y += 1.f;
		std::get<1>(edge).y += 1.f;
		// lerp between the 2 points to get the correct placement of the point
		outPoint = lerp(std::get<0>(edge), std::get<1>(edge), alpha);
		// to make it "lay" above the terrain.
		return true;
	}
	return false;
}

void TerrainDataSystem::makeGrid(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, float resolution, const std::vector<Vertex>& Vertices,
	uint32 LASEntity, uint32 GridEntity, uint32 ContourEntity, ECSManager* ECS)
{

	std::vector<Line> lines;
	std::vector<Vertex> SquareVertices;

	// MAKING GRID
	int xIncrementor = 0;
	int width = 0;
	int yIncrementor = 0;

	MeshComponent* gridMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(GridEntity);
	gridMesh->m_indices.push_back(0);
	for (int i = zMin; i <= zMax; i += resolution, ++yIncrementor)
	{
		xIncrementor = 0;
		for (int j = xMin; j <= xMax; j += resolution, ++xIncrementor)
		{
			Vertex point = Vertex(j, 0.f, i, 0.f, 1.f, 0.f);
			SquareVertices.emplace_back(point);

		
			//gridMesh->m_indices.push_back(gridMesh->m_indices.back()+1);
		}
	}
	width = xIncrementor;
	

	gridMesh->m_vertices = SquareVertices;
	

	auto terrainMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(LASEntity);
	std::vector<Vertex> vertsInSquare;
	std::vector<Vertex> pointsForHeightMap;

	int rowCounter = 1;
	for (int i = 0; i < SquareVertices.size() - (width + 1); ++i) // limit so that last row does not make squares
	{
		if (i == ((width * rowCounter) - 1))
		{
			rowCounter++;
			continue;
			
		}

		const glm::vec2 A = glm::vec2(SquareVertices[i].m_xyz[0], SquareVertices[i].m_xyz[2]);
		const glm::vec2 B = glm::vec2(SquareVertices[i + 1].m_xyz[0], SquareVertices[i + 1].m_xyz[2]);
		const glm::vec2 C = glm::vec2(SquareVertices[i + width + 1].m_xyz[0], SquareVertices[i + width + 1].m_xyz[2]);

		const auto AB = B - A;
		const auto BC = C - B;
		const auto ABxAB = glm::dot(AB, AB);
		const auto BCxBC = glm::dot(BC, BC);

		int pointsInSquare = 0;

		// requires c++ 17 for parallel execution of for loops
		// bottleneck that needed to be optimized due to calling the dot operation multiple times
		std::mutex m;
		std::for_each(
			std::execution::par,
			Vertices.begin(),
			Vertices.end(),
			[A,B,AB,BC,ABxAB,BCxBC, &vertsInSquare, &m](auto&& vert)
			{

				glm::vec2 M = glm::vec2(vert.m_xyz[0], vert.m_xyz[2]);
				glm::vec2 AM = M - A;
				glm::vec2 BM = M - B;
				auto ABxAM = glm::dot(AB, AM);
				auto BCxBM = glm::dot(BC, BM);

				
				if (0 <= ABxAM && ABxAM <= ABxAB &&
					0 <= BCxBM && BCxBM <= BCxBC)
				{
					std::lock_guard<std::mutex> guard(m); // to evade data race
					vertsInSquare.emplace_back(vert);
				}
			});
		//for (const Vertex& vert : Vertices)
		{
		
			
		}
		float avgZ = 0.0f;
		int counter = 0;
		for (const auto& it : vertsInSquare)
		{
			++counter;
			avgZ += it.m_xyz[1];
		}


		float HeightmapX = lerp(SquareVertices[i].m_xyz[0], SquareVertices[i + 1].m_xyz[0], 0.5);
		float HeightmapZ = lerp(SquareVertices[i + 1].m_xyz[2], SquareVertices[i + width + 1].m_xyz[2], 0.5);
		if (vertsInSquare.size() != 0)
		{	
			avgZ = avgZ / (float)counter;
			
			pointsForHeightMap.emplace_back(Vertex(glm::vec3(HeightmapX, avgZ, HeightmapZ),
				glm::vec3(0.f, 1.0f, 0.0f),
				glm::vec2(0.0f, 0.0f)));
		}
		else
		{
			pointsForHeightMap.emplace_back(Vertex(glm::vec3(HeightmapX, yMin, HeightmapZ),
				glm::vec3(0.f, 1.0f, 0.0f),
				glm::vec2(0.0f, 0.0f)));
		}
		
		vertsInSquare.clear();
	}

	terrainMesh->m_vertices = pointsForHeightMap;
	// for each square
	// for each edge in square
	// test is the iso value along the edge
	// e.g. 4 is below 5 but above 3 == true.
	// value needs to be on both sides on  directly on it, skip if same as square vert iso value
	//if true tehn we know we have an intersetion
	// compute intersection point, use interpolation
	// to compute the point between the vertices.
	// if false // else - skip edge
	// line loop GL_LINE_LOOP

	// iso contour
	// 
	width--; // since points are no longer squares there are -1 fewer points per row
	rowCounter = 1;
	float isoValue = yMin + 10;// - 25;
	float ekvidistance = 5.f;
	std::vector<Vertex> contour;
	int cIncrementor = 0;
	auto contourMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(ContourEntity);
	for (int i = 0; i <= pointsForHeightMap.size() - (width + 1); ++i) // skip last row due to making quads, doing 2 rows at the same time
	{
		if (i == ((width * rowCounter)-1))
		{
			rowCounter++;
			continue;

		}

		// check distance or make a map
		// is square adjacent
		// check distance max dist is one full square

		terrainMesh->m_indices.emplace_back((GLuint)i);
		terrainMesh->m_indices.emplace_back((GLuint)i + width);
		terrainMesh->m_indices.emplace_back((GLuint)i + 1);

		terrainMesh->m_indices.emplace_back((GLuint)i + 1);
		terrainMesh->m_indices.emplace_back((GLuint)i + width);
		terrainMesh->m_indices.emplace_back((GLuint)i + width + 1);


		// iso contour
		auto A = glm::vec3(pointsForHeightMap[i].m_xyz[0], pointsForHeightMap[i].m_xyz[1], pointsForHeightMap[i].m_xyz[2]);
		auto B = glm::vec3(pointsForHeightMap[i + 1].m_xyz[0], pointsForHeightMap[i + 1].m_xyz[1], pointsForHeightMap[i + 1].m_xyz[2]);
		auto C = glm::vec3(pointsForHeightMap[i + width + 1].m_xyz[0], pointsForHeightMap[i + width + 1].m_xyz[1], pointsForHeightMap[i + width + 1].m_xyz[2]);
		auto D = glm::vec3(pointsForHeightMap[i + width].m_xyz[0], pointsForHeightMap[i + width].m_xyz[1], pointsForHeightMap[i + width].m_xyz[2]);

		glm::vec3 contourPoint;
		for (int i = isoValue; i < yMax; i+= ekvidistance)
		{
			if (calculatePointForIsoContour(A, B, i, contourPoint))
			{
				contour.emplace_back(contourPoint, glm::vec3(5, 0.f, 0), glm::vec2(0, 0));
				contourMesh->m_indices.emplace_back(GLuint(cIncrementor));
				cIncrementor++;
			}

			if (calculatePointForIsoContour(A, D, i, contourPoint))
			{
				contour.emplace_back(contourPoint, glm::vec3(5, 0.f, 0), glm::vec2(0, 0));
				contourMesh->m_indices.emplace_back(GLuint(cIncrementor));
				cIncrementor++;
			}

			if (calculatePointForIsoContour(C, D, i, contourPoint))
			{
				contour.emplace_back(contourPoint, glm::vec3(5, 0.f, 0), glm::vec2(0, 0));
				contourMesh->m_indices.emplace_back(GLuint(cIncrementor));
				cIncrementor++;

			}

			if (calculatePointForIsoContour(C, B, i, contourPoint))
			{
				contour.emplace_back(contourPoint, glm::vec3(5, 0.f, 0), glm::vec2(0, 0));
				contourMesh->m_indices.emplace_back(GLuint(cIncrementor));
				cIncrementor++;

			}
		}
		
	}

	
	contourMesh->m_vertices = contour;
}

/*
void TerrainDataSystem::drawTerrain(class Shader* shader, const std::string& uniformName, class ECSManager* ECS, MeshComponent& terrainMesh)
{
	TransformComponent transformComp = *ECS->getComponentManager<TransformComponent>()->getComponentChecked(terrainMesh.entityID);
	auto texManager = ECS->getComponentManager<TextureComponent>();
	
	shader->use();
	if (texManager)
	{
		auto texComp = texManager->getComponentChecked(terrainMesh.entityID);
		if (texComp)
		{
			shader->setInt("bUsingTexture", 1);
			glBindTexture(GL_TEXTURE_2D, texComp->textureID);
		}
		else
		{
			shader->setInt("bUsingTexture", 0);
		}
	}
	

	glBindVertexArray(terrainMesh.m_VAO);
	glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
	glDrawElements(terrainMesh.m_drawType, terrainMesh.m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void initialize(GridComponent* gridComp)
{
	/*
	for (int i = 0; i < gridComp.m_lines; ++i)
	{
		glGenVertexArrays(1, &gridComp.m_VAOs[i]);
		glBindVertexArray(gridComp.m_VAOs[i]);

		glGenBuffers(1, &gridComp.m_VBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, gridComp.m_VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Line),
		gridComp.m_vertices.data(), GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, meshComponent.m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));
		glEnableVertexAttribArray(0);

		// 2nd attribute buffer : normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// 3rd attribute buffer : UV.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &meshComponent.m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComponent.m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComponent.m_indices.size() * sizeof(GLuint),
			meshComponent.m_indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		
	//}
}

void TerrainDataSystem::drawGrid(class Shader* shader, const std::string& uniformName, ECSManager* ECS, GridComponent* gridComp)
{
	//shader->use();
	//auto& transformComp = ECS->getComponentManager<TransformComponent>()->getComponent(gridComp.entityID);
	//for (int i = 0; i < gridComp->m_lines.size(); ++i;)// line : gridComp)
	{
		//glBindVertexArray(gridComp.m_VAOs[i]);
		//glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
		//glDrawArrays(gridComp->m_drawType, 0, 2);
		//glBindVertexArray(0);
	}
}
*/
std::tuple<float, float, float, float, float, float> TerrainDataSystem::generateLAS(std::vector<Vertex>& data, MeshComponent* gridMesh, MeshComponent& terrainMesh, ECSManager* ECS)
{


	std::tuple<float, float, float, float, float, float> minMax = getMinMax(data);
	
	std::vector<Vertex> newData;
	float minX = std::get<0>(minMax);
	float maxX = std::get<1>(minMax);
	float minY = std::get<2>(minMax);
	float maxY = std::get<3>(minMax);
	float minZ = std::get<4>(minMax);
	float maxZ = std::get<5>(minMax);

	// Rescalling, min max normalization in range a b
	//x' = a  + ( x - xmin(a*b) /maxX - minX)
	float min = 0.f;
	float max = 1000.f;
	for (const auto& it : data)
	{
		

		newData.emplace_back(Vertex(
			min + (((it.m_xyz[0] - minX) * (max - min)) / (maxX - minX)),
			min + (((it.m_xyz[1] - minY) * (100 - min)) / (maxY - minY)),
			min + (((it.m_xyz[2] - minZ) * (max - min)) / (maxZ - minZ)),
			0.0f, 1.f, 0.f, 0.f, 0.f));
	}
	data = newData;
	return minMax = getMinMax(newData);
	
	//auto gridTrans = ECS->getComponentManager<TransformComponent>().getComponentChecked(gridMesh->entityID);
	//TransformSystem::move(gridMesh->entityID, glm::vec3(-std::get<0>(minMax), 0.0f, std::get<0>(minMax)), ECS);
	//TransformSystem::move(gridMesh->entityID, glm::vec3(-std::get<0>(minMax), 0.0f, std::get<0>(minMax)), ECS);
}

std::vector<Vertex> TerrainDataSystem::readLAS(const std::filesystem::path& filePath)
{
    std::ifstream inputFile;
    inputFile.open(filePath, std::ifstream::in);
    if (!inputFile)
    {
        DEBUG_LOG("Could not open file " + filePath.string());
		return std::vector<Vertex>{};
    }

    std::string line;
    std::string word;
    std::getline(inputFile, line); // skip first line

    std::vector<Vertex> tempVertexPositions{};

    while (std::getline(inputFile, line))
    {

        std::stringstream stringStream;
        stringStream << line;
        word = "";
        stringStream >> word;

        if (stringStream.eof())
            break;

        if (word == "#" || word == "")
            continue; // Line is a comment or blank


        glm::vec3 tempPosition{};
		if (std::isnan((std::stof(word))))
			continue;
		tempPosition.x = (std::stof(word));
        stringStream >> word;
		if (std::isnan((std::stof(word))))
			continue;
        tempPosition.z = (std::stof(word));
        stringStream >> word;
		if (std::isnan((std::stof(word))))
			continue;
        tempPosition.y = (std::stof(word));

		tempVertexPositions.push_back(Vertex(tempPosition, glm::vec3{}, glm::vec2{}));
    }
    inputFile.close();
	return tempVertexPositions;


}

glm::vec3 TerrainDataSystem::getNormalByIndex(uint32 terrainEntity, ECSManager* ECS, int index)
{
	MeshComponent* terrainMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(terrainEntity);

	auto v0 = terrainMesh->m_vertices[terrainMesh->m_indices[index]].m_xyz;
	glm::vec3 A = glm::vec3(v0[0],v0[1],v0[2]);


	auto v1 = terrainMesh->m_vertices[terrainMesh->m_indices[index+1]].m_xyz;
	glm::vec3 B = glm::vec3(v1[0], v1[1], v1[2]);


	auto v2 = terrainMesh->m_vertices[terrainMesh->m_indices[index+2]].m_xyz;
	glm::vec3 C = glm::vec3(v2[0], v2[1], v2[2]);

	glm::vec3 V(B - A);
	glm::vec3 U(C - A);

	glm::vec3 N = glm::cross(V, U);

	return glm::normalize(N);
}
