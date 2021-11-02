#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "MeshSystem.h"
#include "glad/glad.h"
#include "../Components/Components.h"
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

bool MeshSystem::loadMeshLOD(const std::filesystem::path& filePath, MeshComponent& meshComponent, LODMeshType type)
{
    auto& LODMesh = MeshComponent(meshComponent.entityID, meshComponent.ID);
    LODMesh.LODType = type;
    assert(readObj(filePath, LODMesh));

	initialize(LODMesh);
	std::cout << LODMesh.m_indices.size();

	meshComponent.LODMeshes.push_back(LODMesh);
    return true;
}

void MeshSystem::draw(Shader* shader, const std::string& uniformName, class ECSManager* manager, uint32 cameraEntity)
{

    ComponentManager<MeshComponent>* meshManager = manager->getComponentManager<MeshComponent>();
    ComponentManager<TransformComponent>* transformManager = manager->getComponentManager<TransformComponent>();
    TransformComponent* cameraTransform{ manager->getComponentManager<TransformComponent>()->getComponentChecked(cameraEntity) };
    if (!meshManager || !transformManager || !cameraTransform)
        return;


    auto& meshArray = meshManager->getComponentArray();
    shader->use();
    for (auto& meshComp : meshArray)
    {
        // skip transulenct objects
        if (meshComp.bIsTranslucent == true) 
            continue;



        // calculate distance form mesh to camera determine lod to draw
        glm::vec3 cameraPos = glm::vec3(cameraTransform->transform[3]);
        glm::vec3 meshPos= glm::vec3(transformManager->getComponent(meshComp.entityID).transform[3]);
        
        float dist = glm::distance(cameraPos, meshPos);
        // Size doesnt matter
        int LODArraySize = meshComp.LODMeshes.size();
        LODMeshType LODToDraw = LODMeshType::Default;
        if (dist < 8 || LODArraySize == 0)
            LODToDraw = LODMeshType::Default;
        else if (dist < 10 && LODArraySize >= 1)
            LODToDraw = LODMeshType::LOD1;
        else if (dist < 12 && LODArraySize >= 2)
            LODToDraw = LODMeshType::LOD2;
		else if (dist < 120 && LODArraySize >= 3)
			LODToDraw = LODMeshType::LOD3;
        else
            LODToDraw = (LODMeshType)(LODArraySize);

		if (LODToDraw == LODMeshType::Default)
		{
			auto& transformComp = transformManager->getComponent(meshComp.entityID);

			glBindVertexArray(meshComp.m_VAO);
			glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
			glDrawElements(GL_TRIANGLES, meshComp.m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
        else
        {
			for (auto meshToDrawn : meshComp.LODMeshes)
			{
				if (meshToDrawn.LODType == LODToDraw)
				{
					auto& transformComp = transformManager->getComponent(meshToDrawn.entityID);

					glBindVertexArray(meshToDrawn.m_VAO);
					glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
					glDrawElements(GL_TRIANGLES, meshToDrawn.m_indices.size(), GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);

                    break;
				}
			}

        }
    }
}


void MeshSystem::drawOutline(Shader* shader, const std::string& uniformName, ECSManager* manager)
{

	ComponentManager<MeshComponent>* meshManager = manager->getComponentManager<MeshComponent>();
	ComponentManager<TransformComponent>* transformManager = manager->getComponentManager<TransformComponent>();
    ComponentManager<SelectionComponent>* selectionManager = manager->getComponentManager<SelectionComponent>();
	if (!meshManager || !transformManager || !selectionManager)
		return;

    auto& selectionArray = selectionManager->getComponentArray();
	shader->use();
	for (auto& selection : selectionArray)
	{
        for (uint32 id : selection.hitEntities)
        {
            
            MeshComponent& meshComp = *meshManager->getComponentChecked(id);
			auto& transformComp = transformManager->getComponent(meshComp.entityID);

			glm::mat4x4 temp = transformComp.transform;
			glm::mat4x4 tempscale = glm::scale(temp, glm::vec3(1.03f, 1.03f, 1.03f));

			glBindVertexArray(meshComp.m_VAO);
			glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(tempscale));
			glDrawElements(GL_TRIANGLES, meshComp.m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
        }
	}
}

void MeshSystem::drawRTSSelection(Shader* shader, const glm::vec3& startPoint, const glm::vec3& endPoint,
    const std::string& uniformName, MeshComponent& meshComp, TransformComponent& transformComp)
{
    // when using this function meshcomp and transformcomp should already be checked to be valid, see Selection system.

    meshComp.m_vertices.clear();
    meshComp.m_indices.clear();
    meshComp.m_drawType = GL_TRIANGLES;

	float minX = startPoint.x < endPoint.x ? startPoint.x : endPoint.x;
	float minZ = startPoint.z < endPoint.z ? startPoint.z : endPoint.z;
	float maxX = startPoint.x >= endPoint.x ? startPoint.x : endPoint.x;
	float maxZ = startPoint.z >= endPoint.z ? startPoint.z : endPoint.z;

    glm::vec3 MinXZ = glm::vec3(minX, startPoint.y, minZ);
    glm::vec3 MaxXZ = glm::vec3(maxX, startPoint.y, maxZ);
	glm::vec3 MinXMaxZ = glm::vec3(minX, startPoint.y, maxZ);
	glm::vec3 MaxXMinZ = glm::vec3(maxX, startPoint.y, minZ);

    meshComp.m_vertices.push_back(Vertex(MinXZ, glm::vec3(0.f, 1.f, 0.f), glm::vec2{}));
    meshComp.m_vertices.push_back(Vertex(MinXMaxZ, glm::vec3(0.f, 1.f, 0.f), glm::vec2{}));
    meshComp.m_vertices.push_back(Vertex(MaxXMinZ, glm::vec3(0.f, 1.f, 0.f), glm::vec2{}));
    meshComp.m_vertices.push_back(Vertex(MaxXZ, glm::vec3(0.f, 1.f, 0.f), glm::vec2{}));


    //std::vector<int> vec{ 0, 1, 2, 2, 3, 1 };
    meshComp.m_indices.push_back(0);
    meshComp.m_indices.push_back(1);
    meshComp.m_indices.push_back(3);
    meshComp.m_indices.push_back(0);
	meshComp.m_indices.push_back(3);
	meshComp.m_indices.push_back(2);
    initialize(meshComp);


	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();
	glBindVertexArray(meshComp.m_VAO);
	glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformComp.transform));
	glDrawElements(meshComp.m_drawType, meshComp.m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void MeshSystem::drawSelectableEditor(Shader* shader, const std::string& uniformName, class ECSManager* manager)
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
