#pragma once
#include "BaseSystem.h"
#include "glm/glm.hpp"
#include <vector>
#include "../FileSystemHelpers.h"
#include "mono/jit/jit.h"

/// <summary>
/// Handles all HUD. Any HudComponent in existence will be rendered via render.
/// </summary>
class HudSystem : public BaseSystem
{
public:
	static void init(uint32 entity, class ECSManager* ECS, const std::filesystem::path& texturePath);
	static void render(class ECSManager* ECS, class Shader* shader);
	static void setPosition(uint32 entity, const glm::vec2& position, class ECSManager* ECS);
	static void setScale(uint32 entity, const glm::vec2& scale, class ECSManager* ECS);

	static void showHud_Internal(uint32 entity, MonoString* path);
	static void removeHud_Internal(uint32 entity);
};

