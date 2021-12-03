#include "PhysicsSystem.h"
#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include "../ECSManager.h"
#include "TransformSystem.h"
#include "TerrainDataSystem.h"

void PhysicsSystem::UpdatePhysics(ECSManager* ECS, uint32 terrainEntity, float DeltaTime)
{
	auto physArray = ECS->getComponentManager<PhysicsComponent>()->getComponentArray();


	for (PhysicsComponent& pObjet : physArray)
	{
		TransformComponent transComp = ECS->getComponentManager<TransformComponent>()->getComponent(pObjet.entityID);
		glm::vec3 worldPos = transComp.transform[3];

		glm::vec3 normal = TerrainDataSystem::getNormalByIndex(terrainEntity, ECS, 3);

		//a = [nx,ny,nz]*g*ny+[0,-g,0]
		glm::vec3 a = normal * Gravity * normal.y + glm::vec3(0, -Gravity, 0);

		//v = v0 + at
		glm::vec3 v = pObjet.v0 + a * DeltaTime;
		pObjet.v0 = v;

		glm::vec3 s = (pObjet.v0 * DeltaTime + (0.5f * a * DeltaTime * DeltaTime));
		//s = v0 * t + 0.5a * t * t

		TransformSystem::move(pObjet.entityID, s, ECS);
	}

	
}
