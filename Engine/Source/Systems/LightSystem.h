#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

class LightSystem : public BaseSystem
{
public:

	static void InitGBuffer(GBufferComponent* GBufferComp);

};

