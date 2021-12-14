#include "CameraSystem.h"
#include "glm/geometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Input/Input.h"
#include "../ECSManager.h"
#include "../Systems/TransformSystem.h"
#include "../Shader.h"
#include "GLFW/glfw3.h"
#include "../Engine/Engine.h"

#define _USE_MATH_DEFINES // Must be defined to use the M_PI constant
#include <math.h>
//void CameraSystem::setPerspective(uint32 entity, class ECSManager* ECS, const glm::mat4& projectionMatrix)
//{
//
//}

void CameraSystem::setPerspective(uint32 entity, ECSManager* ECS, float fovY, float aspect, float near, float far)
{
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	assert(currentCamera);

	currentCamera->m_projectionMatrix = glm::perspective(glm::radians(fovY), aspect, near, far);
	currentCamera->aspect = aspect;
	currentCamera->far = far;
	currentCamera->near = near;
	currentCamera->fovY = fovY;
}

void CameraSystem::updateGameCamera(uint32 entity, class ECSManager* ECS, float deltaTime)
{
	CameraComponent* currentCamera{ ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity) };
	assert(currentCamera);

	currentCamera->pitch = -50.f;
	currentCamera->yaw = 0.f;
	TransformSystem::setHeight(entity, 20.f, ECS);
	processGameMouseInput(*currentCamera, ECS, deltaTime);
	updateEditorViewMatrix(*currentCamera);
	updateEditorCameraPosition(entity, ECS);
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

void CameraSystem::setPhongUniforms(uint32 entity, Shader* shader, ECSManager* ECS)
{
	shader->use();
	TransformComponent* currentCamera{ ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity) };
	assert(currentCamera);
	shader->setVec3("cameraPosition", glm::vec3(currentCamera->transform[3].x,
		currentCamera->transform[3].y,
		currentCamera->transform[3].z));
}

void CameraSystem::processGameMouseInput(CameraComponent& currentCamera, ECSManager* ECS, float deltaTime)
{
	float screenWidth = Engine::Get().getWindowWidth();
	float screenHeight = Engine::Get().getWindowHeight();
	MousePosition mousePosition = Input::getInstance()->getMousePosition();

	//glm::vec3 forward(getForwardVector(currentCamera));
	//glm::vec3 right(getRightVector(forward));
	glm::vec3 forward(1, 0, 0);
	glm::vec3 right(0, 0, 1);
	//std::cout << "x " << right.x << " y " << right.y << " z " << right.z << std::endl;

	glm::vec3 deltaMovement{};
	//if (mousePosition.x < 50)
	//	deltaMovement -= right * 5.f * deltaTime;
	//if (mousePosition.y < 50)
	//	deltaMovement += forward * 5.f * deltaTime;
	//if (mousePosition.y > (screenHeight-50))
	//	deltaMovement -= forward * 5.f * deltaTime;
	//if (mousePosition.x > (screenWidth-50))
	//	deltaMovement += right * 5.f * deltaTime;

	float speed{ 5 };

	if (mousePosition.x < 50)
		deltaMovement -= right * (float)(50-mousePosition.x) * 0.1f * deltaTime * speed;
	if (mousePosition.y < 50)
		deltaMovement += forward * (float)(50-mousePosition.y) * 0.1f * deltaTime * speed;
	if (mousePosition.y > (screenHeight - 50))
		deltaMovement -= forward * ((screenHeight -mousePosition.y +50)) * 0.1f * deltaTime * speed;
	if (mousePosition.x > (screenWidth - 50))
		deltaMovement -= right * ((screenWidth - mousePosition.x - 50)) * 0.1f * deltaTime * speed;

	TransformSystem::move(currentCamera.entityID, deltaMovement, ECS);
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

void CameraSystem::normalizePlane(glm::vec4& plane)
{
	float mag{std::sqrt(plane.x*plane.x + plane.y*plane.y + plane.z*plane.z)};
	plane.x /= mag;
	plane.y /= mag;
	plane.z /= mag;
	plane.w /= mag;
}

float CameraSystem::isPointInPlane(const glm::vec4& plane, const glm::vec3& point, float radius)
{
	//(frustum_planes.x * pos.x + frustum_planes.y * pos.y + frustum_planes.z * pos.z + frustum_planes.w <= -radius)
	//return (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w) >= -radius;
	//return (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w) <=0.f;
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w);
	//return std::sqrt(plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w) >= -radius;
}

void CameraSystem::createFrustumMesh(uint32 entity, ECSManager* ECS)
{
	ECS->addComponent<MeshComponent>(entity);
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
	CameraComponent* camera = ECS->getComponentManager<CameraComponent>()->getComponentChecked(entity);
	
	assert(camera);
	assert(mesh);
	mesh->bDisregardedDuringFrustumCulling = true;
	mesh->m_drawType = GL_LINES;

	glm::mat4 inverseProjViewMatrix = glm::inverse((camera->m_projectionMatrix * camera->m_viewMatrix));

	std::vector<glm::vec4> positions
	{
		glm::vec4(-1, -1, -1, 1),
		glm::vec4(1, -1, -1, 1),
		glm::vec4(1, 1, -1, 1),
		glm::vec4(-1, 1, -1, 1),
				
		glm::vec4(-1, -1, 1, 1),
		glm::vec4(1, -1, 1, 1),
		glm::vec4(1, 1, 1, 1),
		glm::vec4(-1, 1, 1, 1)
	};

	std::vector<Vertex> vertices;
	for (auto& it : positions)
	{
		it = inverseProjViewMatrix*it; 
		it.x /= it.w;
		it.y /= it.w;
		it.z /= it.w;
		it.w = 1.f;
		mesh->m_vertices.push_back(Vertex(glm::vec3(it), glm::vec3(0, 1, 1), glm::vec2()));
	}

	mesh->m_indices.push_back(0);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(2);
	mesh->m_indices.push_back(2);
	mesh->m_indices.push_back(3);
	mesh->m_indices.push_back(3);
	mesh->m_indices.push_back(0);

	mesh->m_indices.push_back(4);
	mesh->m_indices.push_back(5);
	mesh->m_indices.push_back(5);
	mesh->m_indices.push_back(6);
	mesh->m_indices.push_back(6);
	mesh->m_indices.push_back(7);
	mesh->m_indices.push_back(7);
	mesh->m_indices.push_back(4);

	mesh->m_indices.push_back(0);
	mesh->m_indices.push_back(4);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(5);
	mesh->m_indices.push_back(2);
	mesh->m_indices.push_back(6);
	mesh->m_indices.push_back(3);
	mesh->m_indices.push_back(7);

	MeshSystem::initialize(*mesh);
}
