#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "glm/glm.hpp"

class CollisionSystem : public BaseSystem
{
public:
	/// <summary>
	/// Constructs the collisionComponent
	/// </summary>
	/// <param name="collisionComponent"></param>
	/// <param name="ECS"></param>
	static void construct(struct AxisAlignedBoxComponent& collisionComponent,
		class ECSManager* ECS);

	/// <summary>
	/// Checks if two colliders are colliding
	/// </summary>
	/// <param name="firstCollisionComponent"></param>
	/// <param name="otherCollisionComponent"></param>
	/// <param name="ECS"></param>
	/// <returns>true if there is a collision</returns>
	static bool isColliding(struct AxisAlignedBoxComponent& firstCollisionComponent,
		struct AxisAlignedBoxComponent& otherCollisionComponent, class ECSManager* ECS);

private:
	/// <summary>
	/// Scales the min and max vectors to the outer bounds of the mesh.
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="OUTscaledMin">The lowest position of the mesh</param>
	/// <param name="OUTscaledMax">The highest position of the mesh</param>
	static void scaleToMesh(const struct MeshComponent* mesh,
		glm::vec3 OUTscaledMin, glm::vec3 OUTscaledMax);
};