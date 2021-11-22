#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

class SelectionSystem : public BaseSystem
{
public:

	static void updateSelection(uint32 entity, uint32 cameraEntity, class ECSManager* ECS, float deltaTime);
	static void drawSelectedArea(uint32 entity, class Shader* shader, class ECSManager* ECS);
	static void setHitEntities(uint32 entity, const std::vector<uint32> hitEntities, class ECSManager* ECS);

	static bool SelectionSystem::IsEntitySelected_internal(uint32 EntityID);
private:
	static void init(uint32 entity, class ECSManager* ECS);
};

