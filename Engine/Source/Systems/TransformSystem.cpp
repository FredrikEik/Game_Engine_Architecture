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
