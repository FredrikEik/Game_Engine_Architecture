#include "PhysicsSystem.h"
#include "../ECSManager.h"
#include "TerrainSystem.h"
#include "TransformSystem.h"
#include "../Engine/Engine.h"
void PhysicsSystem::update(uint32 terrainEntity, ECSManager* ECS, float deltaTime)
{
	if (!ECS->getComponentManager<PhysicsComponent>())
		return;
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
	float tempRadius{ 0.8 };
	for (auto& it : physicsComponents)
	{
		TransformComponent* entityTransform = transformManager->getComponentChecked(it.entityID);
		assert(entityTransform);
		tempHeight = TerrainSystem::getHeight(*entityTransform, *terrain, tempTerrainIndex);
		tempIsInAir = entityTransform->transform[3].y > (tempHeight+ tempRadius+0.0001);

		// If airborne there is no surface normal. 
		tempSurfaceNormal = TerrainSystem::getNormal(*entityTransform, *terrain, tempTerrainIndex) * (float)!tempIsInAir;
		if (tempTerrainIndex < 0)
			tempSurfaceNormal = glm::normalize(it.velocity*-1.f);


		it.acceleration = getAcceleration(tempSurfaceNormal, it.mass);
		it.velocity += it.acceleration * deltaTime;

		if (tempTerrainIndex < 0)
		{
			tempSurfaceNormal = glm::normalize(it.velocity * -1.f);
			tempCollisionNormal = getCollisionNormal(it.velocity, tempSurfaceNormal);
			float speed = glm::length(it.velocity) > 5.f ? glm::length(it.velocity) : 5;
			it.velocity = tempCollisionNormal * speed;
		}
		else if (tempTerrainIndex != it.lastTriangleIndex && !tempIsInAir)
		{
			tempCollisionNormal = getCollisionNormal(it.velocity, tempSurfaceNormal);
			if(it.bIsInAir) // if entity was in air last frame
				it.velocity = tempCollisionNormal * glm::length(it.velocity) * it.restitution;
			else
				it.velocity = tempCollisionNormal * glm::length(it.velocity);
			glm::vec3 frictionDirection = glm::normalize(it.velocity) * -1.f;
			it.velocity += (it.friction * frictionDirection * glm::length(it.velocity));
		}

		//if (!tempIsInAir)
		//{
		//	glm::vec3 frictionDirection = glm::normalize(it.velocity) * -1.f;
		//	it.velocity += (it.friction * frictionDirection * glm::length(it.velocity));
		//}

		// s = V_0 * t + 1/2*a*t*t
		TransformSystem::move(*entityTransform, 
			(it.velocity*deltaTime + 0.5f * glm::vec3(0, -core::GRAVITY,0)*deltaTime*deltaTime));

		tempHeight = TerrainSystem::getHeight(*entityTransform, *terrain, tempTerrainIndex);
		bool isUnderGround = entityTransform->transform[3].y < (tempHeight + tempRadius+0.0001);
		if (isUnderGround)
			entityTransform->transform[3].y = tempHeight+ tempRadius;

		it.lastSurfaceNormal = tempSurfaceNormal;
		it.lastTriangleIndex = tempIsInAir ? -1 : tempTerrainIndex;
		it.bIsInAir = tempIsInAir;
	}
}

void PhysicsSystem::setVelocity(uint32 entity, const glm::vec3& newVelocity, ECSManager* ECS, bool isAdditive)
{
	PhysicsComponent* component = ECS->getComponentManager<PhysicsComponent>()->getComponentChecked(entity);
	component->velocity = isAdditive ? component->velocity + newVelocity : newVelocity;
}

void PhysicsSystem::setVelocity_Internal(uint32 entity, glm::vec3 newVelocity, bool isAdditive)
{
	setVelocity(entity, newVelocity, Engine::Get().getECSManager(), isAdditive);
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

glm::vec3 PhysicsSystem::getCollisionNormal(const glm::vec3& velocity, const glm::vec3& surfaceNormal)
{
	float dot = glm::dot(velocity, surfaceNormal);
	return glm::normalize(velocity - 2.f * dot * surfaceNormal);
}
