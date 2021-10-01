#include "CameraSystem.h"
#include "glm/geometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Input/Input.h"
#include "../ECSManager.h"
#include "../Systems/TransformSystem.h"

#define _USE_MATH_DEFINES // Must be defined to use the M_PI constant
#include <math.h>
void CameraSystem::setPerspective(CameraComponent& currentCamera, 
	int verticalAngle, float aspectRatio, float nearPlane, float farPlane)
{
	
	currentCamera.m_projectionMatrix = glm::perspective<float>
					(verticalAngle, aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(CameraComponent& currentCamera, 
						const TransformComponent& transform)
{
	assert(false); // Implement this before you try to use it maybe?
}

void CameraSystem::updateEditorCamera(uint32 entity, class ECSManager* ECS, float deltaTime)
{
	CameraComponent *currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	assert(currentCamera);

	processEditorMouseInput(*currentCamera);
	processEditorKeyboardInput(entity, ECS, deltaTime);
	updateEditorViewMatrix(*currentCamera);
}

void CameraSystem::processEditorKeyboardInput(uint32 entity, class ECSManager* ECS, float deltaTime)
{
	Input* input{ Input::getInstance() };

	TransformComponent* currentTransform{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity) };
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };

	glm::vec3 forward(getForwardVector(*currentCamera));
	glm::vec3 right(getRightVector(forward));
	glm::vec3 up(getUpVector(forward, right));

	// TODO: This does not really belong in camera system
	glm::vec3 deltaMovement{};

	// TODO: Make sure this works
	if (input->getKeyState(KEY_W).bHeld)
		deltaMovement += forward * 10.f * deltaTime;
	if (input->getKeyState(KEY_S).bHeld)
		deltaMovement -= forward * 10.f * deltaTime;
	if (input->getKeyState(KEY_D).bHeld)
		deltaMovement += right * 10.f * deltaTime;
	if (input->getKeyState(KEY_A).bHeld)
		deltaMovement -= right * 10.f * deltaTime;
	if (input->getKeyState(KEY_E).bHeld)
		deltaMovement += up * 10.f * deltaTime;
	if (input->getKeyState(KEY_Q).bHeld)
		deltaMovement -= up * 10.f * deltaTime;

	TransformSystem::move(entity, deltaMovement, ECS);
}

void CameraSystem::processEditorMouseInput(CameraComponent& currentCamera)
{
	Input* input{ Input::getInstance() };

	if (!input->getMouseKeyState(KEY_RMB).bHeld)
		return;

	const MousePosition mousePosition{ input->getMousePosition() };
	pitch(mousePosition.deltaY, currentCamera);
	yaw(mousePosition.deltaX, currentCamera);

}

void CameraSystem::pitch(float deltaPitch, CameraComponent& currentCamera)
{
	currentCamera.pitch += deltaPitch;

	if (currentCamera.pitch > 89.f)
		currentCamera.pitch = 89.f;
	if (currentCamera.pitch < -89.f)
		currentCamera.pitch = -89.f;

}

void CameraSystem::yaw(float deltaYaw, CameraComponent& currentCamera)
{
	currentCamera.yaw += deltaYaw;

}

void CameraSystem::updateEditorViewMatrix(CameraComponent& currentCamera)
{
	glm::vec3 forward(getForwardVector(currentCamera));
	glm::vec3 right(getRightVector(forward));
	glm::vec3 up(getUpVector(forward, right));

	currentCamera.m_viewMatrix = glm::mat4
	(
		right.x,	right.y,	right.z,	0,
		up.x,		up.y,		up.z,		0,
		forward.x,	forward.y,	forward.z,	0,
		0,			0,			0,			1.f
	);
}

glm::vec3 CameraSystem::getForwardVector(const CameraComponent& currentCamera)
{
	float yawRadians{ currentCamera.yaw * (float)(M_PI / 180.f) };
	float pitchRadians{ currentCamera.pitch * (float)(M_PI / 180.f) };

	return glm::normalize(glm::vec3(
		std::cos(yawRadians) * std::cos(pitchRadians),
		std::sin(pitchRadians),
		std::sin(yawRadians) * std::cos(pitchRadians)));
}

glm::vec3 CameraSystem::getRightVector(const glm::vec3& forwardVector)
{
	return glm::normalize(glm::cross(glm::vec3(0,1,0), forwardVector));
}

glm::vec3 CameraSystem::getUpVector(const glm::vec3& forwardVector, const glm::vec3& upVector)
{
	return glm::normalize(glm::cross(forwardVector, upVector));
}

void CameraSystem::updateEditorCameraPosition(CameraComponent& currentCamera, 
	const TransformComponent& transform)
{
	// TODO: Figure out if this should translate negative ;)
	glm::translate(currentCamera.m_viewMatrix, -glm::vec3(transform.transform[3]));
}
