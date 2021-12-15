#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include "../Components/Components.h"

/// <summary>
/// Camera class handles everything that has to do with camera
/// Setting uniforms for shaders, set the view and update camera
/// Check frustum
/// </summary>
/// <seealso cref="BaseSystem" />
class CameraSystem : public BaseSystem
{
public:

	/// <summary>
	/// Sets the perspective.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="fovY">The fov y.</param>
	/// <param name="aspect">The aspect.</param>
	/// <param name="">The .</param>
	/// <param name="">The .</param>
	static void setPerspective(uint32 entity, class ECSManager* ECS,
		float fovY, float aspect, float near, float far );
	/// <summary>
	/// Updates the game camera.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="deltaTime">The delta time.</param>
	static void updateGameCamera(uint32 entity, class ECSManager* ECS, float deltaTime);

	/// <summary>
	/// Updates the editor camera.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	/// <param name="deltaTime">The delta time.</param>
	static void updateEditorCamera(uint32 entity, class ECSManager* ECS, float deltaTime);

	/// <summary>
	/// Draws the specified entity.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="shader">The shader.</param>
	/// <param name="ECS">The ecs.</param>
	static void draw(uint32 entity, class Shader* shader, class ECSManager* ECS);
	/// <summary>
	/// Sets the phong uniforms.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="shader">The shader.</param>
	/// <param name="ECS">The ecs.</param>
	static void setPhongUniforms(uint32 entity, class Shader* shader, class ECSManager* ECS);

	/// <summary>
	/// Gets the forward vector.
	/// </summary>
	/// <param name="currentCamera">The current camera.</param>
	/// <returns></returns>
	static glm::vec3 getForwardVector(const CameraComponent& currentCamera);
	/// <summary>
	/// Gets the right vector.
	/// </summary>
	/// <param name="forwardVector">The forward vector.</param>
	/// <returns></returns>
	static glm::vec3 getRightVector(const glm::vec3& forwardVector);
	/// <summary>
	/// Gets up vector.
	/// </summary>
	/// <param name="forwardVector">The forward vector.</param>
	/// <param name="upVector">Up vector.</param>
	/// <returns></returns>
	static glm::vec3 getUpVector(const glm::vec3& forwardVector, const glm::vec3& upVector);
	/// <summary>
	/// Normalizes the plane.
	/// </summary>
	/// <param name="OUTplane">The ou tplane.</param>
	static void normalizePlane(glm::vec4& OUTplane);
	/// <summary>
	/// Determines whether [is point in plane] [the specified plane].
	/// </summary>
	/// <param name="plane">The plane.</param>
	/// <param name="point">The point.</param>
	/// <param name="radius">The radius.</param>
	/// <returns></returns>
	static float isPointInPlane(const glm::vec4& plane, const glm::vec3& point, float radius);
	
	/// <summary>
	/// Withins the frustum.
	/// </summary>
	/// <param name="a">a.</param>
	/// <param name="b">The b.</param>
	/// <param name="c">The c.</param>
	/// <returns></returns>
	static bool WithinFrustum(const float& a, const float& b, const float& c) { return a <= b && b <= c; }

	/// <summary>
	/// Creates the frustum mesh.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	static void createFrustumMesh(uint32 entity, class ECSManager* ECS);

private:
	// GAME
/// <summary>
/// Processes the game mouse input.
/// </summary>
/// <param name="currentCamera">The current camera.</param>
/// <param name="ECS">The ecs.</param>
/// <param name="deltaTime">The delta time.</param>
	static void processGameMouseInput(CameraComponent& currentCamera, ECSManager* ECS, float deltaTime);
	// End of game -----------------------------------------

private:
	// EDITOR
/// <summary>
/// Processes the editor keyboard input.
/// </summary>
/// <param name="entity">The entity.</param>
/// <param name="ECS">The ecs.</param>
/// <param name="deltaTime">The delta time.</param>
	static void processEditorKeyboardInput(uint32 entity, class ECSManager* ECS, float deltaTime);
	/// <summary>
	/// Processes the editor mouse input.
	/// </summary>
	/// <param name="currentCamera">The current camera.</param>
	static void processEditorMouseInput(CameraComponent& currentCamera);

	// If this is made public, it would have to update the view matrix as well
/// <summary>
/// Pitches the specified delta pitch.
/// </summary>
/// <param name="deltaPitch">The delta pitch.</param>
/// <param name="currentCamera">The current camera.</param>
	static void pitch(float deltaPitch, CameraComponent& currentCamera);
	// If this is made public, it would have to update the view matrix as well
/// <summary>
/// Yaws the specified delta yaw.
/// </summary>
/// <param name="deltaYaw">The delta yaw.</param>
/// <param name="currentCamera">The current camera.</param>
	static void yaw(float deltaYaw, CameraComponent& currentCamera);

	/// <summary>
	/// Updates the editor view matrix.
	/// </summary>
	/// <param name="currentCamera">The current camera.</param>
	static void updateEditorViewMatrix(CameraComponent& currentCamera);

private:
	/// <summary>
	/// Updates the editor camera position.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="ECS">The ecs.</param>
	static void updateEditorCameraPosition(uint32 entity, class ECSManager* ECS);
	// End of editor -----------------------------------------
};