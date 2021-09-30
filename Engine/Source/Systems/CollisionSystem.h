#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "glm/glm.hpp"

class CollisionSystem : public BaseSystem
{
public:
	static void construct(struct AxisAlignedBoxComponent& collisionComponent,
		class ECSManager* ECS);

	static bool isColliding(struct AxisAlignedBoxComponent& collisionComponent,
		struct AxisAlignedBoxComponent& otherCollisionComponent);

private:
	static void scaleToMesh(const struct MeshComponent* mesh,
		glm::vec3 OUTscaledMin, glm::vec3 OUTscaledMax);
};