#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <filesystem>
#include <map>

class TextureSystem final : public BaseSystem
{
public:
	static void loadImage(int32 entity, const std::filesystem::path& filePath, struct TextureComponent* textureComponent);
	static void loadMaterial(struct MaterialComponent* MaterialComp, std::map<std::string, std::string> nameFileMap);
};

