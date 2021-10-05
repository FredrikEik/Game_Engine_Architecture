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

// TODO: Test this function
bool CollisionSystem::isColliding(AxisAlignedBoxComponent& firstCollisionComponent,
	AxisAlignedBoxComponent& otherCollisionComponent, ECSManager* ECS)
{
	// Get the transform components for each entity
	auto transformManager = ECS->getComponentManager<TransformComponent>();
	const TransformComponent& firstTransform = transformManager->
		getComponent(firstCollisionComponent.entityID);
	const TransformComponent& otherTransform = transformManager->
		getComponent(otherCollisionComponent.entityID);
	// -----------------------------------------------------------------

	// Calculate the positions of each collider
	glm::vec3 firstPosition{ firstCollisionComponent.center + 
		glm::vec3(firstTransform.transform[3]) };

	glm::vec3 otherPosition{ otherCollisionComponent.center +
		glm::vec3(otherTransform.transform[3])};
	// -----------------------------------------------------------------

	// Check if we collide along the x axis
	if ((firstPosition.x + firstCollisionComponent.maxScaled.x) >= 
		(otherPosition.x + otherCollisionComponent.minScaled.x) &&
		(otherPosition.x + otherCollisionComponent.maxScaled.x) >= 
		(firstPosition.x + otherCollisionComponent.minScaled.x))
		;
	else
		return false;
	// -----------------------------------------------------------------

	// If we need collision in the Y axis, make sure this does not break the collision overall. 
	// Check if we collide along the y axis
	//if ((firstPosition.y + firstCollisionComponent.maxScaled.y) >=
	//	(otherPosition.y + otherCollisionComponent.minScaled.y) &&
	//	(otherPosition.y + otherCollisionComponent.maxScaled.y) >=
	//	(firstPosition.y + otherCollisionComponent.minScaled.y))
	//	;
	//else
	//	return false;
	// -----------------------------------------------------------------

	// Check if we collide along the z axis
	if ((firstPosition.z + firstCollisionComponent.maxScaled.z) >=
		(otherPosition.z + otherCollisionComponent.minScaled.z) &&
		(otherPosition.z + otherCollisionComponent.maxScaled.z) >=
		(firstPosition.z + otherCollisionComponent.minScaled.z))
		;
	else
		return false;
	// -----------------------------------------------------------------

	return true;
}

void CollisionSystem::testCollision(uint32 entityA, uint32 entityB, ECSManager* ECS)
{
	assert(false); // Implement
}

void CollisionSystem::scaleToMesh(const MeshComponent* mesh, 
	glm::vec3 OUTscaledMin, glm::vec3 OUTscaledMax)
{
	const auto& vertices = mesh->m_vertices;

	// Making sure that OUTscaledMin is within the mesh.
	OUTscaledMin = glm::vec3(vertices[0].m_xyz[0],
		vertices[0].m_xyz[1],
		vertices[0].m_xyz[2]);

	// Making sure that OUTscaledMax is within the mesh.
	OUTscaledMax = glm::vec3(vertices[0].m_xyz[0],
		vertices[0].m_xyz[1],
		vertices[0].m_xyz[2]);

	// Looping through all vertices to find the outer bounds
	// of the mesh.
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
