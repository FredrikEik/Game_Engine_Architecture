#include "PhysicsSystem.h"
#include "../ECSManager.h"
#include "TerrainSystem.h"
#include "TransformSystem.h"
void PhysicsSystem::update(uint32 terrainEntity, ECSManager* ECS, float deltaTime)
{
	std::vector<PhysicsComponent>& physicsComponents = ECS->getComponentManager<PhysicsComponent>()->getComponentArray();
	auto transformManager = ECS->getComponentManager<TransformComponent>();
	MeshComponent* terrain = ECS->getComponentManager<MeshComponent>()->getComponentChecked(terrainEntity);

	assert(terrain);
	assert(transformManager);

	int32 tempTerrainIndex{};
	float tempHeight{};
	bool tempIsInAir{};
	glm::vec3 tempSurfaceNormal{};
	glm::vec3 tempCollisionNormal{};
	for (auto& it : physicsComponents)
	{
		TransformComponent* entityTransform = transformManager->getComponentChecked(it.entityID);
		assert(entityTransform);
		tempHeight = TerrainSystem::getHeight(*entityTransform, *terrain, tempTerrainIndex);
		tempIsInAir = entityTransform->transform[3].y > (tempHeight+ 0.0001);

		tempSurfaceNormal = TerrainSystem::getNormal(*entityTransform, *terrain, tempTerrainIndex) * (float)!tempIsInAir;

		//it.velocity = getVelocity(it.velocity,
		//	getAcceleration(tempSurfaceNormal, it.mass), // TEST
		//	deltaTime);	
		//it.acceleration = 
		//it.velocity.y -= core::GRAVITY *  deltaTime;
		it.velocity += getAcceleration(tempSurfaceNormal, it.mass) *  deltaTime;

		if (tempTerrainIndex != it.lastTriangleIndex && !tempIsInAir)
		{
			//tempSurfaceNormal = TerrainSystem::getNormal(*entityTransform, *terrain, tempTerrainIndex);

			//tempCollisionNormal = 
			//	(it.lastSurfaceNormal + tempSurfaceNormal) / glm::length(it.lastSurfaceNormal + tempSurfaceNormal);
			//it.velocity = getVelocity(it.velocity, glm::normalize(tempCollisionNormal));
			tempCollisionNormal = getCollisionNormal(it.velocity, tempSurfaceNormal);
			it.velocity = tempCollisionNormal * glm::length(it.velocity) * it.restitution;
		}
		

		// s = V_0 * t + 1/2*a*t*t
		TransformSystem::move(*entityTransform, (it.velocity*deltaTime + 0.5f * glm::vec3(0, -core::GRAVITY,0)*deltaTime*deltaTime));
		//TransformSystem::move(*entityTransform, it.velocity);
		tempHeight = TerrainSystem::getHeight(*entityTransform, *terrain, tempTerrainIndex);
		bool isUnderGround = entityTransform->transform[3].y < (tempHeight + 0.0001);
		if (isUnderGround)
			entityTransform->transform[3].y = tempHeight;

		it.lastSurfaceNormal = tempSurfaceNormal;
		it.lastTriangleIndex = tempIsInAir ? -1 : tempTerrainIndex;
		it.bIsInAir = tempIsInAir;
	}
}

glm::vec3 PhysicsSystem::getAcceleration(const glm::vec3& surfaceNormal, const float& mass)
{
	return (1.f / mass) * 
		(((mass * (core::GRAVITY) * surfaceNormal * surfaceNormal.y) 
			+ mass * glm::vec3(0, -core::GRAVITY, 0)));
}

glm::vec3 PhysicsSystem::getVelocity(const glm::vec3& currentVelocity, const glm::vec3& currentAcceleration, const float& deltaTime)
{
	return currentVelocity + (currentAcceleration * deltaTime);
}

glm::vec3 PhysicsSystem::getVelocity(const glm::vec3& currentVelocity, const glm::vec3& collisionNormal)
{
	return currentVelocity - (2.f * (currentVelocity * collisionNormal) * collisionNormal);
}

glm::vec3 PhysicsSystem::getCollisionNormal(const glm::vec3& velocity, const glm::vec3& surfaceNormal)
{
	float dot = glm::dot(velocity, surfaceNormal);
	return glm::normalize(velocity - 2.f * dot * surfaceNormal);
}
