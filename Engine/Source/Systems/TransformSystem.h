#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

class TransformSystem final: public BaseSystem
{
public:
	static void moveAll(class ComponentManager<class TransformComponent>* componentManager);
};