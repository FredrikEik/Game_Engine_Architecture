#include "CameraSystem.h"
#include "glm/geometric.hpp"
#include <glm/gtc/matrix_transform.hpp>

void CameraSystem::setPerspective(CameraComponent& currentCamera, 
	int verticalAngle, float aspectRatio, float nearPlane, float farPlane)
{
	
	currentCamera.m_projectionMatrix = glm::perspective<float>
					(verticalAngle, aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(CameraComponent& currentCamera, 
						const TransformComponent& transform)
{

}
