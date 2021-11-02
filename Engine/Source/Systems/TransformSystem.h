#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

class TransformSystem final: public BaseSystem
{
public:
	static void moveAll(class ComponentManager<class TransformComponent>* componentManager);
	static void move(uint32 entity, glm::vec3 deltaLocation, class ECSManager* ECS);
	static void setPosition(uint32 entity, glm::vec3 newLocation, class ECSManager* ECS);
	static void setHeight(uint32 entity, float newHeight, class ECSManager* ECS);
};