#pragma once
#include "BaseSystem.h"
#include "../Components/Components.h"

/// <summary>
/// LightSystem, deals with shadows, lighs and deferred rendering
/// </summary>
/// <seealso cref="BaseSystem" />
class LightSystem : public BaseSystem
{
public:

	/// <summary>
	/// Initializes the s buffer.
	/// </summary>
	/// <param name="SComp">The s comp.</param>
	static void InitGBuffer(GBufferComponent* GBufferComp);
	/// <summary>
	/// Defers the rendering.
	/// </summary>
	/// <param name="GeometryPassShader">The geometry pass shader.</param>
	/// <param name="LightPassShader">The light pass shader.</param>
	/// <param name="uniformName">Name of the uniform.</param>
	/// <param name="manager">The manager.</param>
	/// <param name="SystemEntity">The system entity.</param>
	/// <param name="cameraEntity">The camera entity.</param>
	static void DeferredRendering(class Shader* GeometryPassShader, class Shader* LightPassShader, const std::string& uniformName,
		class ECSManager* manager, uint32 SystemEntity, uint32 cameraEntity);


	static void SetLightValues(uint32 entityID, class ECSManager* ECS, glm::vec3 lightColor, float linear, float quadratic);


	static void InitSBuffer(ShadowBufferComponent* SComp);

	
	static void DrawShadows(Shader* ShadowShader, Shader* ShadowDepthShader, const std::string& uniformName, class ECSManager* ECS, uint32 SystemEntity, uint32 cameraEntity, float deltaTime);
};

