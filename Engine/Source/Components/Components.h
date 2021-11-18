#pragma once
#include "../CoreMinimal.h"
#include "glad/glad.h"
#include <vector>
#include "../Vertex.h"
#include "glm/glm.hpp"
#include "../Assets/DefaultAssets.h"

/*		ATTENTION
*		ATTENTION
*		ATTENTION	
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*
* Whenever you create a new component you need to add it
* to the function removeComponentByRTTI in ECSManager.cpp
* 
* If the component is reusable, you need to add it 
* to the function assignAsset and loadAsset in Factory.h
*
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*		ATTENTION
*/


struct Component 
{
public:
	Component(uint32 entity, uint32 componentID) : entityID{ entity }, ID{ componentID } {}
	~Component(){}
	uint32 entityID; // TODO: Should probably be uint16
	uint32 ID;
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
	std::size_t hash{};
	GLuint m_VAO{};
	GLuint m_VBO{};
	GLuint m_EBO{};
	GLenum m_drawType{ GL_TRIANGLES };
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<MeshComponent> LODMeshes;

	LODMeshType LODType{ LODMeshType::Default };
	bool bIsTranslucent{ false };
	bool bDisregardedDuringFrustumCulling{ false }; // True if it should not be considered for frustum culling. Renders
	bool bShouldRender{ true };
};


struct CameraComponent final : public Component
{
	CameraComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::mat4x4 m_viewMatrix{};
	glm::mat4x4 m_projectionMatrix{};
	float yaw{};
	float pitch{};
	float fovY{};
	float near{};
	float far{};
	float aspect{};
};

struct TransformComponent final : public Component
{
	TransformComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::mat4x4 transform{ glm::mat4(1.0f) };
};

struct AxisAlignedBoxComponent final : public Component
{
	AxisAlignedBoxComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	glm::vec3 minScaled{};
	glm::vec3 maxScaled{};
	glm::vec3 center{};
};


struct SelectionComponent final : public Component
{
	SelectionComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::vec3 initialHitPos{};
	glm::vec3 currentHitPos{};

	std::vector<uint32> hitEntities;
};

struct SphereComponent final : public Component
{
	SphereComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	float radius{};
	glm::vec3 center{};
};

struct TextureComponent final : public Component
{
	TextureComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	std::size_t hash{};

	int width{};
	int height{};
	int numberOfChannels{};
	GLenum wrapMode{ GL_REPEAT };
	uint textureID{};

	uint8_t* rgbImage{};
};

struct SoundComponent final : public Component
{
	SoundComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	std::size_t hash{};

	uint32 p_Source{};
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	glm::vec3 position{};
	glm::vec3 velocity{};
	bool loopSound{false};
	uint32 buffer{}; // Todo: Can be a vector of all the buffers

};