#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <filesystem>
#include <map>
#include "mono/jit/jit.h"

/// <summary>
/// Texture system loads textures for texture components and materials
/// </summary>
/// <seealso cref="BaseSystem" />
class TextureSystem final : public BaseSystem
{
public:
	/// <summary>
	/// Loads the image.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="filePath">The file path.</param>
	/// <param name="textureComponent">The texture component.</param>
	static void loadImage(int32 entity, const std::filesystem::path& filePath, struct TextureComponent* textureComponent);

	/// <summary>
	/// Loads the material.
	/// </summary>
	/// <param name="MaterialComp">The material comp.</param>
	/// <param name="nameFileMap">The name file map.</param>
	static void loadMaterial(struct MaterialComponent* MaterialComp, const std::map<std::string, std::string>& nameFileMap);

	/// <summary>
	/// Loads the image with alpha.
	/// </summary>
	/// <param name="filePath">The file path.</param>
	/// <param name="textureComponent">The texture component.</param>
	static void loadImageWithAlpha(const std::filesystem::path& filePath, struct TextureComponent* textureComponent);

	/// <summary>
	/// Adds the material texture internal.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="uniform">The uniform.</param>
	/// <param name="path">The path.</param>
	static void addMaterialTexture_Internal(uint32 entity, MonoString* uniform, MonoString* path);
};

