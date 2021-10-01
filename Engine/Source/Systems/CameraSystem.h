#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "../Components/Components.h"

// TODO: Implement draw

class CameraSystem : public BaseSystem
{
public:
	static void setPerspective(CameraComponent& currentCamera, int verticalAngle, 
				float aspectRatio, float nearPlane, float farPlane);
	static void update(CameraComponent& currentCamera, const TransformComponent& transform);

	static void updateEditorCamera(uint32 entity, class ECSManager* ECS, float deltaTime);

	// Add functionality to update forward/right/up vector
	// Add pitching

private:
	static void processEditorKeyboardInput(uint32 entity, class ECSManager* ECS, float deltaTime);
	static void processEditorMouseInput(CameraComponent& currentCamera);

	// If this is made public, it would have to update the view matrix as well
	static void pitch(float deltaPitch, CameraComponent& currentCamera);
	// If this is made public, it would have to update the view matrix as well
	static void yaw(float deltaYaw, CameraComponent& currentCamera);

	static void updateEditorViewMatrix(CameraComponent& currentCamera);
	static glm::vec3 getForwardVector(const CameraComponent& currentCamera);
	static glm::vec3 getRightVector(const glm::vec3& forwardVector);
	static glm::vec3 getUpVector(const glm::vec3& forwardVector, const glm::vec3& upVector);
	static void updateEditorCameraPosition(CameraComponent& currentCamera, 
		const TransformComponent& transform);
};