#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

class LightSystem : public BaseSystem
{
public:

	static void InitGBuffer(GBufferComponent* GBufferComp);
	static void DefferedRendering(class Shader* GeometryPassShader, class Shader* LightPassShader, const std::string& uniformName,
		class ECSManager* manager, uint32 SystemEntity, uint32 cameraEntity);
};

