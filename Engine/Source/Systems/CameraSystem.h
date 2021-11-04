#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "../Components/Components.h"

// TODO: Implement draw

class CameraSystem : public BaseSystem
{
public:
/*	static void setPerspective(uint32 entity, class ECSManager* ECS, 
		const glm::mat4& projectionMatrix );*/	
	static void setPerspective(uint32 entity, class ECSManager* ECS, 
		float fovY, float aspect, float near, float far );
	static void updateGameCamera(uint32 entity, class ECSManager* ECS, float deltaTime);

	static void updateEditorCamera(uint32 entity, class ECSManager* ECS, float deltaTime);

	static void draw(uint32 entity, class Shader* shader, class ECSManager* ECS);

	static glm::vec3 getForwardVector(const CameraComponent& currentCamera);
	static glm::vec3 getRightVector(const glm::vec3& forwardVector);
	static glm::vec3 getUpVector(const glm::vec3& forwardVector, const glm::vec3& upVector);
	static void normalizePlane(glm::vec4& OUTplane);
	static float isPointInPlane(const glm::vec4& plane, const glm::vec3& point, float radius);
	// Add functionality to update forward/right/up vector
	// Add pitching

private:
	// GAME
	static void processGameMouseInput(CameraComponent& currentCamera, ECSManager* ECS, float deltaTime);
	// End of game -----------------------------------------

private:
	// EDITOR
	static void processEditorKeyboardInput(uint32 entity, class ECSManager* ECS, float deltaTime);
	static void processEditorMouseInput(CameraComponent& currentCamera);

	// If this is made public, it would have to update the view matrix as well
	static void pitch(float deltaPitch, CameraComponent& currentCamera);
	// If this is made public, it would have to update the view matrix as well
	static void yaw(float deltaYaw, CameraComponent& currentCamera);

	static void updateEditorViewMatrix(CameraComponent& currentCamera);

private:
	static void updateEditorCameraPosition(uint32 entity, class ECSManager* ECS);
	// End of editor -----------------------------------------
};