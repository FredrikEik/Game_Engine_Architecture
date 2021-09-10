#pragma once
#include "../CoreMinimal.h"
#include "glad/glad.h"
#include <vector>
#include "../Vertex.h"
#include "glm/glm.hpp"

// TODO: screw the constructor in component. 
// Consider that every component initialize their own ids

struct Component 
{
public:
	Component(uint32 entity, uint32 componentID) : entityID{ entity }, ID{ componentID } {}
	~Component(){}
	uint32 ID;
	uint32 entityID;
};

//struct UniqueComponent : public Component
//{
//public:
//	UniqueComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
//	~UniqueComponent() {}
//
//};

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
	std::size_t hash{};
	GLuint m_VAO{};
	GLuint m_VBO{};
	GLuint m_EBO{};
	GLenum m_drawType{ GL_TRIANGLES };
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
};

struct CameraComponent final : public Component
{
	CameraComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::mat4x4 m_viewMatrix{};
	glm::mat4x4 m_projectionMatrix{};
};

struct TransformComponent final : public Component
{
	TransformComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::mat4x4 transform{ glm::mat4(1.0f) };
};