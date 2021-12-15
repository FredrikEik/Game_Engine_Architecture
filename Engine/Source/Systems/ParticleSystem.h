#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"


/// <summary>
/// The Particle System is responsible for handling everything except UI regarding the ParticleComponent
/// </summary>
/// <seealso cref="ParticleComponent" />
/// <seealso cref="ParticleWidget" />
class ParticleSystem : public BaseSystem
{
public:

	/// <summary>
	/// Initializes a ParticleComponent for the entity.
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="maxParticles">The maximum particles.</param>
	/// <param name="path">The texture path.</param>
	/// <param name="textureRows">The texture rows.</param>
	static void init(uint32 entityID, class ECSManager* ECS, uint32 maxParticles = 1,
		std::filesystem::path path = "", int textureRows = 1);


	/// <summary>
	/// Creates a quad mesh and sets up all buffers for the ParticleComponent
	/// </summary>
	/// <param name="emitter">The emitter.</param>
	/// <param name="maxParticles">The maximum particles.</param>
	static void initMesh(class ParticleComponent* emitter, uint32 maxParticles);

	/// <summary>
	/// Initializes the ParticleComponent's texture.
	/// </summary>
	/// <param name="emitter">The emitter.</param>
	/// <param name="path">The path.</param>
	/// <param name="textureRows">The texture rows.</param>
	static void initTexture(class ParticleComponent* emitter, const std::filesystem::path& path, int8 textureRows);

	/// <summary>
	/// Updates and renders all active ParticleComponent's 
	/// </summary>
	/// <param name="cameraEntity">The camera entity.</param>
	/// <param name="shader">The shader.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="deltaTime">The delta time.</param>
	static void update(uint32 cameraEntity, class Shader* shader, class ECSManager* ECS, float deltaTime);

	/// <summary>
	/// Sets the particle active and resets the ParticleComponent
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="bShouldBeActive">if set to <c>true</c> [b should be active].</param>
	/// <param name="ECS">The ecs.</param>
	static void setParticleActive(uint32 entityID, bool bShouldBeActive, ECSManager* ECS);
	static void setParticleActive(class ParticleComponent* emitter, bool bShouldBeActive);
	static void setParticleActive_Internal(uint32 entityID, bool bShouldBeActive);
private:
	/// <summary>
	/// Spawns all particles that should be spawned this frame.
	/// </summary>
	/// <param name="emitter">The emitter.</param>
	/// <param name="deltaTime">The delta time.</param>
	/// <param name="emitterLocation">The emitter location.</param>
	/// <param name="cameraLocation">The camera location.</param>
	static void spawnParticles(struct ParticleComponent& emitter, float deltaTime,
		const glm::vec3& emitterLocation, const glm::vec3& cameraLocation);

	/// <summary>
	/// Sorts the emitters. The emitter furthest away will be in front.
	/// </summary>
	/// <param name="ECS">The ecs.</param>
	/// <param name="cameraPosition">The camera position.</param>
	/// <param name="emitters">The emitters.</param>
	/// <param name="OUTsortedEmitters">The sorted array, with the index to the furthest emitter in front.</param>
	static void sortEmitters(ECSManager* ECS, const glm::vec3& cameraPosition,
		const std::vector<ParticleComponent>& emitters,
		std::vector <int>& OUTsortedEmitters);

	/// <summary>
	/// Updates a single particle
	/// </summary>
	/// <param name="i">The index in the loop.</param>
	/// <param name="emitter">The emitter.</param>
	/// <param name="particle">The particle.</param>
	/// <param name="emitterPosition">The emitter position.</param>
	/// <param name="cameraPosition">The camera position.</param>
	/// <param name="deltaTime">The delta time.</param>
	/// <returns>False when reaching the first inactive particle, stop updating.</returns>
	static bool updateParticle(const uint32& i, ParticleComponent& emitter, ParticleComponent::Particle& particle,
		const glm::vec3& emitterPosition, const glm::vec3& cameraPosition, const float& deltaTime);


	/// <summary>
	/// Renders the specified emitter, binding the texture and setting uniforms.
	/// </summary>
	/// <param name="emitter">The emitter.</param>
	/// <param name="shader">The shader.</param>
	static void render(const ParticleComponent& emitter, class Shader* shader);
};

