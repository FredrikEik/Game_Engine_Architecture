#pragma once
#include "../CoreMinimal.h"
#include "glad/glad.h"
#include <vector>
#include "../Vertex.h"
#include "glm/glm.hpp"

struct Component 
{
public:
	Component(uint32 entity, uint32 componentID) : entityID{ entity }, ID{ componentID } {}
	~Component(){}
	uint32 ID;
	uint32 entityID;
};

struct testComponent : public Component
{
public:
	testComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	//testComponent(const testComponent& other) : Component(other) {}
	~testComponent(){}
	float pos[3]{};
};

struct MeshComponent final : public Component
{
public:
	MeshComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	GLuint m_VAO{};
	GLuint m_VBO{};
	GLuint m_EBO{};
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	GLenum m_drawType{ GL_TRIANGLES };
};

struct TransformComponent final : public Component
{
	TransformComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::mat4x4 transform{};
};