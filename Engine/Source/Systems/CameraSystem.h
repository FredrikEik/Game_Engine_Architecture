#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "../Components/Components.h"

class CameraSystem : public BaseSystem
{
public:
	void setPerspective(CameraComponent& currentCamera, int verticalAngle, 
				float aspectRatio, float nearPlane, float farPlane);
	void update(CameraComponent& currentCamera, const TransformComponent& transform);

	// Add functionality to update forward/right/up vector
	// Add pitching
};