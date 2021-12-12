#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <filesystem>

class TextureSystem final : public BaseSystem
{
public:
	static void loadImage(int32 entity, const std::filesystem::path& filePath, struct TextureComponent* textureComponent);
	static void loadImageWithAlpha(const std::filesystem::path& filePath, struct TextureComponent* textureComponent);
};

