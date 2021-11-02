#include "TransformSystem.h"
#include "../Components/ComponentManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECSManager.h"

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

void TransformSystem::setPosition(uint32 entity, glm::vec3 newLocation, ECSManager* ECS)
{
	TransformComponent* transform = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	transform->transform[3] = glm::vec4(newLocation, 1);
}