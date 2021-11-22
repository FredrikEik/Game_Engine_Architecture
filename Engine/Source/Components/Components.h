#pragma once
#include "../CoreMinimal.h"
#include "glad/glad.h"
#include <vector>
#include "../Vertex.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include "../Assets/DefaultAssets.h"
#include "../JSON/json.hpp"

using JSON = nlohmann::json;

// TODO: Create a python script or something that can automate the process below

#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/reflection.h>


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
* 
* If the component should be loaded / saved, you must implement json and jsonParse
* As well as add it to the createComponent function in Load.cpp
* Aaaaas well as add it to addComponentToJson() in Save.cpp
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
	
	virtual JSON json() { return JSON(); } // TODO: Make these pure virtual
	virtual void jsonParse(const JSON& json) {}
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
	std::string path{};
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

	JSON json() override;
	void jsonParse(const JSON& json) override;
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

	JSON json() override;
	void jsonParse(const JSON& json) override;
};

struct AxisAlignedBoxComponent final : public Component
{
	AxisAlignedBoxComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	glm::vec3 minScaled{};
	glm::vec3 maxScaled{};
	glm::vec3 center{};
	bool bShouldGenerateOverlapEvents{ true };

	JSON json() override;
	void jsonParse(const JSON& json) override;
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
	std::string path{};

	int width{};
	int height{};
	int numberOfChannels{};
	GLenum wrapMode{ GL_REPEAT };
	uint textureID{};

	uint8_t* rgbImage{};


	JSON json() override;
	void jsonParse(const JSON& json) override;
};


struct ScriptComponent final : public Component
{
	ScriptComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}
	
	std::string ScriptClassName{ "Unit" };
	MonoClass* m_Class{};
	MonoObject* m_Object{};
	MonoClassField* entityID_handle{};
	
	//std::unordered_map<size_t, Ref<ScriptField>> m_Fields;
	std::unordered_map<size_t, MonoMethod*> m_Methods;
	//std::unordered_map<size_t, Ref<ScriptProperty>> m_Properties;


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