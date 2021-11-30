#pragma once
#include "BaseSystem.h"
#include "glm/glm.hpp"
class PhysicsSystem : public BaseSystem
{
public:
	static void update(uint32 terrainEntity, class ECSManager* ECS, float deltaTime);

private:
	static glm::vec3 getAcceleration(const glm::vec3& surfaceNormal, const float& mass);
	static glm::vec3 getVelocity(const glm::vec3& currentVelocity, 
		const glm::vec3& currentAcceleration, const float& deltaTime);
	static glm::vec3 getVelocity(const glm::vec3& currentVelocity,
		const glm::vec3& collisionNormal);
	static glm::vec3 getCollisionNormal(const glm::vec3& velocity, const glm::vec3& surfaceNormal);
};

