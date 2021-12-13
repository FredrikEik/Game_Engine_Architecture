#include "TransformSystem.h"
#include "../Components/ComponentManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECSManager.h"
#include "../Engine/Engine.h"

void TransformSystem::moveAll(class ComponentManager<TransformComponent>* componentManager)
{
	//ComponentManager<TransformComponent>* componentManager = manager->getComponentManager<TransformComponent>();
	if (!componentManager)
		return;
	auto& transformArray = componentManager->getComponentArray();

	for (uint32 i{}; i < transformArray.size(); ++i)
	{
		transformArray[i].transform = glm::translate(transformArray[i].transform, glm::vec3(.0010f, .003f, 0.001f));
	}
}

void TransformSystem::move(uint32 entity, glm::vec3 deltaLocation, ECSManager* ECS)
{
	TransformComponent* transform = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	transform->transform[3] += glm::vec4(deltaLocation, 0);
}

void TransformSystem::move(TransformComponent& OUTtransform, glm::vec3 deltaLocation)
{
	OUTtransform.transform[3] += glm::vec4(deltaLocation, 0);
}

void TransformSystem::move_internal(uint32 entity, glm::vec3 newLocation)
{
	TransformComponent* transformComp = Engine::Get().getECSManager()->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	transformComp->transform[3] += glm::vec4(newLocation, 0);
}

glm::vec3 TransformSystem::getTransform_internal(uint32 entity)
{
	TransformComponent* transformComp = Engine::Get().getECSManager()->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	return transformComp->transform[3];
}

glm::vec3 TransformSystem::setPosition_internal(uint32 entity, glm::vec3 newLocation)
{
	TransformComponent* transformComp = Engine::Get().getECSManager()->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	return transformComp->transform[3] = glm::vec4(newLocation,1);
}

void TransformSystem::setPosition(uint32 entity, glm::vec3 newLocation, ECSManager* ECS)
{
	TransformComponent* transform = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	transform->transform[3] = glm::vec4(newLocation, 1);
}

void TransformSystem::setScale(int32 entity, glm::vec3 newScale, ECSManager* ECS)
{
	glm::mat4x4& transform = ECS->getComponentManager<TransformComponent>()->getComponent(entity).transform;
	//float scale[3] = { transform[0].x, transform[1].y, transform[2].z };
	transform[0].x = newScale[0];
	transform[1].y = newScale[1];
	transform[2].z = newScale[2];
}

void TransformSystem::setHeight(uint32 entity, float newHeight, ECSManager* ECS)
{
	TransformComponent* transform = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	transform->transform[3].y = newHeight;
}
