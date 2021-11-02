#pragma once
#include "BaseSystem.h"

class SphereSystem : public BaseSystem
{
public:
	static void construct(uint32 entityID, class ECSManager* ECS);
};

