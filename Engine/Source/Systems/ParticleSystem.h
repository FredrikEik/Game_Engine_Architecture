#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"


class ParticleSystem : public BaseSystem
{
public:
	static void init(uint32 entityID, uint32 maxParticles, uint32 spawnRate, 
		const ParticleComponent::ParticleBlueprint& particleBlueprint, class ECSManager* ECS);

	static void initMesh(class ParticleComponent* emitter, uint32 maxParticles);
	static void initTexture(class ParticleComponent* emitter, const std::filesystem::path& path, int8 textureRows);

	static void update(uint32 cameraEntity, class Shader* shader, class ECSManager* ECS, float deltaTime);

private:
	static void constructQuad(struct MeshComponent& mesh);
	static void spawnParticles(struct ParticleComponent& emitter, float deltaTime,
		const glm::vec3& emitterLocation, const glm::vec3& cameraLocation);
};

