#include "CollisionSystem.h"
#include "../ECSManager.h"
#include "../Components/Components.h"
#include "../Components/ComponentManager.h"
#include "../Vertex.h"

void CollisionSystem::construct(struct AxisAlignedBoxComponent& collisionComponent,
	class ECSManager* ECS)
{
	auto transformManager = ECS->getComponentManager<TransformComponent>();
	auto meshManager = ECS->getComponentManager<MeshComponent>();
	uint32 entityID = collisionComponent.entityID;
	const TransformComponent& transform = transformManager->getComponent(entityID);
	const MeshComponent* mesh = meshManager->getComponentChecked(entityID);

	// TODO: test if this works for the default case of a tiny cube collision
	glm::vec3 min{-0.5f, -0.5f, -0.5f};
	glm::vec3 max{0.5f, 0.5f, 0.5f};

	if (mesh)
		scaleToMesh(mesh, min, max);

	glm::vec3 scale = glm::vec3(transform.transform[0][0],
		transform.transform[1][1],
		transform.transform[2][2]);
	min *= scale;
	max *= scale;

	collisionComponent.minScaled = min;
	collisionComponent.maxScaled = max;
	collisionComponent.center = glm::vec3((max.x - min.x) / 2.f,
		(max.y - min.y) / 2.f, (max.z - min.z) / 2.f);
}

bool CollisionSystem::isColliding(AxisAlignedBoxComponent& collisionComponent, 
	AxisAlignedBoxComponent& otherCollisionComponent)
{
	// TODO: Implement this
	return false;
}

void CollisionSystem::scaleToMesh(const MeshComponent* mesh, 
	glm::vec3 OUTscaledMin, glm::vec3 OUTscaledMax)
{
	auto vertices = mesh->m_vertices;
	glm::vec3 tempPos{};
	for (const auto& it : vertices)
	{
		tempPos = glm::vec3(it.m_xyz[0], it.m_xyz[0], it.m_xyz[0]);
		if (tempPos.x < OUTscaledMin.x) OUTscaledMin.x = (tempPos.x);
		if (tempPos.y < OUTscaledMin.y) OUTscaledMin.y = (tempPos.y);
		if (tempPos.z < OUTscaledMin.z) OUTscaledMin.z =(tempPos.z);

		if (tempPos.x > OUTscaledMax.x) OUTscaledMax.x = (tempPos.x);
		if (tempPos.y > OUTscaledMax.y) OUTscaledMax.y = (tempPos.y);
		if (tempPos.z > OUTscaledMax.z) OUTscaledMax.z = (tempPos.z);
	}
}
