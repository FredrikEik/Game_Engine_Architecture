#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "MeshSystem.h"
#include "glad/glad.h"
#include "../Components/Components.h"
#include "glm/glm.hpp"
#include "../Assets/DefaultAssets.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include "../ECSManager.h"
#include "../Shader.h"


bool MeshSystem::loadMesh(const std::filesystem::path& filePath, MeshComponent& meshComponent)
{
    assert(readObj(filePath, meshComponent));

    initialize(meshComponent);
    std::cout << meshComponent.m_indices.size();
	return true;
}

void MeshSystem::draw(Shader* shader, const std::string& uniformName, class ECSManager* manager)
{

    ComponentManager<MeshComponent>* meshManager = manager->getComponentManager<MeshComponent>();
    ComponentManager<TransformComponent>* transformManager = manager->getComponentManager<TransformComponent>();
    if (!meshManager || !transformManager)
        return;

    auto& meshArray = meshManager->getComponentArray();
    shader->use();
    for (auto& meshComp : meshArray)
    {
        auto& transformComp = transformManager->getComponent(meshComp.entityID);
        
        glBindVertexArray(meshComp.m_VAO); 
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
        glDrawElements(meshComp.m_drawType, meshComp.m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void MeshSystem::drawSelectable(Shader* shader, const std::string& uniformName, class ECSManager* manager)
{

	ComponentManager<MeshComponent>* meshManager = manager->getComponentManager<MeshComponent>();
	ComponentManager<TransformComponent>* transformManager = manager->getComponentManager<TransformComponent>();
	if (!meshManager || !transformManager)
		return;

	auto& meshArray = meshManager->getComponentArray();
	shader->use();
	for (auto& meshComp : meshArray)
	{
		auto& transformComp = transformManager->getComponent(meshComp.entityID);
		int r = (meshComp.entityID & 0x000000FF) >> 0;
		int g = (meshComp.entityID & 0x0000FF00) >> 8;
		int b = (meshComp.entityID & 0x00FF0000) >> 16;
		

		glBindVertexArray(meshComp.m_VAO);
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
		glUniform4f(glGetUniformLocation(shader->getShaderID(), "u_pickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		glDrawElements(meshComp.m_drawType, meshComp.m_indices.size(), GL_UNSIGNED_INT, 0);
		

        glBindVertexArray(0);
	}
}


void MeshSystem::copyMesh(const MeshComponent& meshToCopy, MeshComponent& newMesh)
{
    newMesh.hash = meshToCopy.hash;
    newMesh.m_drawType = meshToCopy.m_drawType;
    newMesh.m_EBO = meshToCopy.m_EBO;
    newMesh.m_indices = meshToCopy.m_indices;
    newMesh.m_VAO = meshToCopy.m_VAO;
    newMesh.m_VBO = meshToCopy.m_VBO;
    newMesh.m_vertices = meshToCopy.m_vertices;
}

void MeshSystem::initialize(MeshComponent& meshComponent)
{
	glGenVertexArrays(1, &meshComponent.m_VAO);
	glBindVertexArray(meshComponent.m_VAO);

    glGenBuffers(1, &meshComponent.m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshComponent.m_VBO);
    glBufferData(GL_ARRAY_BUFFER, meshComponent.m_vertices.size() * sizeof(Vertex), 
                meshComponent.m_vertices.data(), GL_STATIC_DRAW);

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
}

bool MeshSystem::readObj(const std::filesystem::path& filePath, MeshComponent& meshComponent)
{
    std::ifstream inputFile;
    inputFile.open(filePath, std::ifstream::in);
    if (!inputFile)
    {
        DEBUG_LOG("Could not open file");
        return false;
    }

    std::string line;
    std::string word;

    std::vector<glm::vec3> tempVertexPositions{};
    std::vector<glm::vec3> tempVertexNormals{};
    std::vector<glm::vec2> tempVertexUVs{};

    // TODO: Figure out if this is worth it.
    std::unordered_map<ObjTriangleIndex, unsigned int, HashObjTriangleIndex> indexMap;
    unsigned int elementIndex{};

    while (std::getline(inputFile, line))
    {
        std::stringstream stringStream;
        stringStream << line;
        word = "";
        stringStream >> word;

        if (word == "#" || word == "")
            continue; // Line is a comment or blank

        if (word == "v")
        {
            glm::vec3 tempPosition{};
            stringStream >> word;
            tempPosition.x = (std::stof(word));
            stringStream >> word;
            tempPosition.y = (std::stof(word));
            stringStream >> word;
            tempPosition.z = (std::stof(word));

            tempVertexPositions.push_back(tempPosition);
        }

        if (word == "vn")
        {
            glm::vec3 tempNormal{};
            stringStream >> word;
            tempNormal.x = (std::stof(word));
            stringStream >> word;
            tempNormal.y = (std::stof(word));
            stringStream >> word;
            tempNormal.z = (std::stof(word));

            tempVertexNormals.push_back(tempNormal);
        }

        if (word == "vt")
        {
            glm::vec2 tempUV{};
            stringStream >> word;
            tempUV.x = (std::stof(word));
            stringStream >> word;
            tempUV.y = (std::stof(word));

            tempVertexUVs.push_back(tempUV);
        }

        if (word == "f")
        {
            // TODO: Figure out if quad or tri

            ObjTriangleIndex objTriangleIndex{};
            for (int i{}; i < 3; ++i)
            {
                stringStream >> word;

                std::stringstream tempWord(word);
                std::string triangleIndexSegment{};
                std::vector<std::string> segmentArray{};
                while (std::getline(tempWord, triangleIndexSegment, '/'))
                    segmentArray.push_back(triangleIndexSegment);

                objTriangleIndex.posIndex = std::stoi(segmentArray[0]) - 1;

                // uv
                if (segmentArray[1] != "")
                    objTriangleIndex.uvIndex = std::stoi(segmentArray[1]) - 1;
                else
                    objTriangleIndex.uvIndex = -1;

                objTriangleIndex.normalIndex = std::stoi(segmentArray[2]);

                if (indexMap.find(objTriangleIndex) != indexMap.end())
                {
                    // If the map already contains the vertex, just push the index
                    meshComponent.m_indices.push_back(indexMap.find(objTriangleIndex)->second);
                }
                else
                {
                    // If the map does not contain the vertex, create it and add it to the map
                    glm::vec2 tempUV;

                    if (objTriangleIndex.uvIndex >= 0)
                        tempUV = tempVertexUVs[static_cast<unsigned int>(objTriangleIndex.uvIndex)];
                    else
                        tempUV = glm::vec2(0, 0);

                    meshComponent.m_vertices.push_back(Vertex(tempVertexPositions[static_cast<unsigned int>(objTriangleIndex.posIndex)],
                        tempVertexNormals[static_cast<unsigned int>(objTriangleIndex.normalIndex)],
                        tempUV));

                    meshComponent.m_indices.push_back(elementIndex);
                    indexMap.emplace(std::pair<ObjTriangleIndex, unsigned int>(objTriangleIndex, elementIndex));
                    ++elementIndex;
                }
            }
        }
    }

    inputFile.close();
    return true;
}
