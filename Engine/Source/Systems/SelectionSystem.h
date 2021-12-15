#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

/// <summary>
/// Selection system enables the player to use the RTS style selection during the game.
/// Selection system does collision checks, ray cast, draws square when dragging.
/// </summary>
/// <seealso cref="BaseSystem" />
class SelectionSystem : public BaseSystem
{
public:

	/// <summary>
	/// Updates the selection.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="cameraEntity">The camera entity.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="deltaTime">The delta time.</param>
	static void updateSelection(uint32 entity, uint32 cameraEntity, class ECSManager* ECS, float deltaTime);
	/// <summary>
	/// Draws the selected area.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="shader">The shader.</param>
	/// <param name="ECS">The ecs.</param>
	static void drawSelectedArea(uint32 entity, class Shader* shader, class ECSManager* ECS);
	/// <summary>
	/// Sets the hit entities.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="hitEntities">The hit entities.</param>
	/// <param name="ECS">The ecs.</param>
	static void setHitEntities(uint32 entity, const std::vector<uint32> hitEntities, class ECSManager* ECS);

	/// <summary>
	/// Determines whether [is entity selected internal] [the specified entity identifier].
	/// </summary>
	/// <param name="EntityID">The entity identifier.</param>
	/// <returns>
	///   <c>true</c> if [is entity selected internal] [the specified entity identifier]; otherwise, <c>false</c>.
	/// </returns>
	static bool SelectionSystem::IsEntitySelected_internal(uint32 EntityID);
	

	/// <summary>
	/// Gets the cursor world position.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="cameraEntity">The camera entity.</param>
	/// <param name="ECS">The ecs.</param>
	/// <returns></returns>
	static glm::vec3 getCursorWorldPosition(uint32 entity, uint32 cameraEntity, class ECSManager* ECS);
	/// <summary>
	/// Gets the cursor world position internal.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <returns></returns>
	static glm::vec3 getCursorWorldPosition_Internal(uint32 entity);
private:
	/// <summary>
	/// Initializes the specified entity.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	static void init(uint32 entity, class ECSManager* ECS);
};

