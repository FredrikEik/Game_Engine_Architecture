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

		it.velocity = getVelocity(it.velocity,
			getAcceleration(tempSurfaceNormal, it.mass), // TEST
			deltaTime);	
		//TransformSystem::move(*entityTransform, it.velocity);
		//tempHeight = TerrainSystem::getHeight(*entityTransform, *terrain, tempTerrainIndex);
		//tempIsInAir = entityTransform->transform[3].y > (tempHeight + 0.0001);
		if (tempTerrainIndex != it.lastTriangleIndex && !tempIsInAir)
		{
			//if (it.lastTriangleIndex == -1)
			//	it.lastSurfaceNormal = glm::vec(0, -1, 0);
			tempCollisionNormal = 
				(it.lastSurfaceNormal + tempSurfaceNormal) / glm::length(it.lastSurfaceNormal + tempSurfaceNormal);
			it.velocity = getVelocity(it.velocity, glm::normalize(tempCollisionNormal));
			//it.velocity = getVelocity(it.velocity, glm::normalize(tempSurfaceNormal + it.velocity));
		}
		


		//if(glm::length(it.velocity) > 0.1)
		TransformSystem::move(*entityTransform, it.velocity);
		if (!tempIsInAir)
			entityTransform->transform[3].y = tempHeight;
		//else
		//{

		//}
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
