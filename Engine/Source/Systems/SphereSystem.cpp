#include "SphereSystem.h"
#include "../ECSManager.h"
#include "CollisionSystem.h"

void SphereSystem::construct(uint32 entityID, ECSManager* ECS)
{
	auto transformManager = ECS->getComponentManager<TransformComponent>();
	auto meshManager = ECS->getComponentManager<MeshComponent>();
	auto collisionComponent = ECS->getComponentManager<SphereComponent>()->getComponentChecked(entityID);

	//uint32 entityID = collisionComponent->entityID;
	const TransformComponent& transform = transformManager->getComponent(entityID);
	const MeshComponent* mesh = meshManager->getComponentChecked(entityID);

	// TODO: test if this works for the default case of a tiny cube collision
	glm::vec3 min{ -0.5f, -0.5f, -0.5f };
	glm::vec3 max{ 0.5f, 0.5f, 0.5f };

	if (mesh)
	 	CollisionSystem::scaleToMesh(mesh, min, max);

	glm::vec3 scale = glm::vec3(transform.transform[0][0],
		transform.transform[1][1],
		transform.transform[2][2]);
	min *= scale;
	max *= scale;


	glm::vec3 center = glm::vec3((max.x - min.x) / 2.f,
		(max.y - min.y) / 2.f, (max.z - min.z) / 2.f);
	collisionComponent->center = center;
	float radius = std::sqrt(center.x * center.x + center.y * center.y + center.z * center.z);
	collisionComponent->radius = radius;
}
