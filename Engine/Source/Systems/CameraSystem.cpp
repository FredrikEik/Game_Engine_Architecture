#include "CameraSystem.h"
#include "glm/geometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Input/Input.h"
#include "../ECSManager.h"
#include "../Systems/TransformSystem.h"
#include "../Shader.h"
#include "GLFW/glfw3.h"

#define _USE_MATH_DEFINES // Must be defined to use the M_PI constant
#include <math.h>
void CameraSystem::setPerspective(uint32 entity, class ECSManager* ECS, const glm::mat4& projectionMatrix)
{
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	assert(currentCamera);

	currentCamera->m_projectionMatrix = projectionMatrix;
}

void CameraSystem::update(uint32 entity, class ECSManager* ECS)
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
	updateEditorCameraPosition(entity, ECS);
}

void CameraSystem::draw(uint32 entity, Shader* shader, ECSManager* ECS)
{
	shader->use();
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	assert(currentCamera);

	shader->setMat4("u_view", currentCamera->m_viewMatrix);
	shader->setMat4("u_projection", currentCamera->m_projectionMatrix);

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
		deltaMovement -= right * 10.f * deltaTime;
	if (input->getKeyState(KEY_A).bHeld)
		deltaMovement += right * 10.f * deltaTime;
	if (input->getKeyState(KEY_E).bHeld)
		deltaMovement += glm::vec3(0,1,0) * 10.f * deltaTime;
	if (input->getKeyState(KEY_Q).bHeld)
		deltaMovement -= glm::vec3(0, 1, 0) * 10.f * deltaTime;

	TransformSystem::move(entity, deltaMovement, ECS);
	//std::cout << currentTransform->transform[3].x << " " 
	//	<< currentTransform->transform[3].y<< " " 
	//	<< currentTransform->transform[3].z << "\n";
}

void CameraSystem::processEditorMouseInput(CameraComponent& currentCamera)
{
	Input* input{ Input::getInstance() };

	if (!input->getMouseKeyState(KEY_RMB).bHeld)
		return;

	const MousePosition mousePosition{ input->getMousePosition() };
	yaw(mousePosition.deltaX, currentCamera);
	pitch(-mousePosition.deltaY, currentCamera);

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

	currentCamera.m_viewMatrix = glm::lookAt(glm::vec3(), forward, up);

	//currentCamera.m_viewMatrix = glm::mat4
	//(
	//	right.x,	right.y,	right.z,	0,
	//	up.x,		up.y,		up.z,		0,
	//	forward.x,	forward.y,	forward.z,	0,
	//	0,			0,			0,			1.f
	//);
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

void CameraSystem::updateEditorCameraPosition(uint32 entity, class ECSManager* ECS)
{
	// TODO: Figure out if this should translate negative ;)
	TransformComponent* currentTransform{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity) };
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	currentCamera->m_viewMatrix = glm::translate(currentCamera->m_viewMatrix, -glm::vec3(currentTransform->transform[3]));
}
