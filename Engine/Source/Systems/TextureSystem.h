#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <filesystem>
#include <map>
#include "mono/jit/jit.h"

class TextureSystem final : public BaseSystem
{
public:
	static void loadImage(int32 entity, const std::filesystem::path& filePath, struct TextureComponent* textureComponent);

	static void loadMaterial(struct MaterialComponent* MaterialComp, const std::map<std::string, std::string>& nameFileMap);

	static void loadImageWithAlpha(const std::filesystem::path& filePath, struct TextureComponent* textureComponent);

	static void addMaterialTexture_Internal(uint32 entity, MonoString* uniform, MonoString* path);
};

