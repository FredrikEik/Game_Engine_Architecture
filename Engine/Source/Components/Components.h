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
	
	/// <summary>
	/// Formats the component as JSON. Make sure jsonParse reflects this
	/// If not implemented, the component will not be saved.
	/// </summary>
	/// <returns>JSON object of all the data a component has</returns>
	virtual JSON json() { return JSON(); }

	/// <summary>
	/// Parses the JSON and sets data accordingly. Make sure it reflects json()
	/// If not implemented, the component cannot be loaded
	/// </summary>
	/// <param name="json">The json.</param>
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

struct PhysicsComponent final : public Component
{
	PhysicsComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	glm::vec3 acceleration{};
	glm::vec3 velocity{};
	glm::vec3 lastSurfaceNormal{};
	int32 lastTriangleIndex{};
	float mass{ 1.f };
	//float baryCentricErrorMargin{0.985f};
	bool bIsInAir{ false };
	float restitution{ 0.75 }; // How bouncy/elastic it is.
};

struct TrailComponent final : public Component
{
	TrailComponent(uint32 entity, uint32 componentID) : Component(entity, componentID) {}

	std::vector<std::vector<glm::vec3>> positions;

	float lastRecordedTime{};
	float recordInterval{ 0.5f };
	bool bRecording{ false };
};


/// <summary>
/// The Particle Component acts as a typical particle emitter when given to an entity
/// </summary>
struct ParticleComponent final : public Component
{
	ParticleComponent(uint32 entity, uint32 componentID) : Component(entity, componentID),
		mesh(entity, componentID), texture(entity, componentID){}


	/// <summary>
	/// This is the actual particle data. Should be one instance per particle
	/// </summary>
	struct Particle
	{
		glm::vec4 startColor{1,0,0,1}, endColor{1,0,0,1};
		glm::vec3 position{};
		glm::vec3 velocity{};
		glm::vec3 acceleration{};
		float totalLife{}, currentLife{}, cameraDistance{ -100000.f }, startSize{ 1 }, endSize{ 1 };
		float textureIndex{ 1 };
		bool active{ false };
		bool operator<(const Particle& other)
		{
			return (this->active && this->cameraDistance > other.cameraDistance) || (this->active && !other.active);
			//return (this->cameraDistance > other.cameraDistance);
		}
	};


	/// <summary>
	/// This is the template new particles are spawned from. 
	/// If only Particle is initialized, all particles spawned will be identical. 
	/// If any of the offsets are set, every new particle will spawn with the Particle data + random value between min and max
	/// </summary>
	struct ParticleBlueprint
	{
		Particle particle;
		glm::vec3 positionMinOffset{}, positionMaxOffset{};
		glm::vec3 velocityMinOffset{}, velocityMaxOffset{};
		glm::vec3 accelerationMinOffset{}, accelerationMaxOffset{};
		glm::vec4 colorMinOffset{}, colorMaxOffset{};
		float sizeMinOffset{ }, sizeMaxOffset{ };
		float lifeMinOffset{ }, lifeMaxOffset{ };
	};

	MeshComponent mesh;
	TextureComponent texture;

	std::vector<Particle> particles;
	std::vector<float> positionData;
	std::vector<float> colorData;
	std::vector<float> lifeAndSizeData;

	int textureRows{ 1 };

	uint32 maxParticles{1};
	uint32 activeParticles{};
	uint32 lastUsedParticle{};
	uint32 spawnRate{};

	float spawnFrequency{};
	float timeSinceLastSpawn{};
	float emitterLifeTime{};
	float emitterTotalLifeTime{};
	bool bLoops{ false };

	GLenum blendSFactor{ GL_SRC_ALPHA };
	GLenum blendDFactor{ GL_ONE_MINUS_SRC_ALPHA };

	ParticleBlueprint particleBlueprint;

	GLuint positionBuffer{};
	GLuint colorBuffer{};
	GLuint lifeAndSizeDataBuffer{};

	JSON json() override;
	void jsonParse(const JSON& json) override;
};
