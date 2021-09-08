#pragma once
#include "../CoreMinimal.h"
#include "glad/glad.h"
#include <vector>
#include "../Vertex.h"

class Component 
{
public:
	Component(uint32 entity, uint32 componentID) : entityID{ entity }, ID{ componentID } {}
	~Component(){}
	uint32 ID;
	uint32 entityID;
};

class testComponent final : public Component 
{
public:
	testComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	//testComponent(const testComponent& other) : Component(other) {}
	~testComponent(){}
	float pos[3]{};
};

class MeshComponent final : public Component
{
	MeshComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	GLuint m_VAO{};
	GLuint m_VBO{};
	GLuint m_EAB{};
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	GLenum m_drawType{ GL_TRIANGLES };
};