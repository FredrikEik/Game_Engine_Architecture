#include "TextureSystem.h"
#include "../Components/Components.h"
#include "../ECSManager.h"
#include "../Components/ComponentManager.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
void TextureSystem::loadImage(int32 entity, const std::filesystem::path& filePath, TextureComponent* textureComponent)
{
	//TextureComponent* textureComponent = ECS->getComponentManager<TextureComponent>()->getComponentChecked(entity);
	assert(textureComponent);

	textureComponent->rgbImage = stbi_load(filePath.generic_string().c_str(), 
				&textureComponent->width, &textureComponent->height, &textureComponent->numberOfChannels, 0);

	assert(textureComponent->rgbImage); // A bit harsh maybe. Implement graceful failing of image loading

	glGenTextures(1, &textureComponent->textureID);
	glBindTexture(GL_TEXTURE_2D, textureComponent->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureComponent->width, textureComponent->height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureComponent->rgbImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureComponent->rgbImage);
}

void TextureSystem::loadImageWithAlpha(const std::filesystem::path& filePath, TextureComponent* textureComponent)
{
	assert(textureComponent);

	textureComponent->rgbImage = stbi_load(filePath.generic_string().c_str(),
		&textureComponent->width, &textureComponent->height, &textureComponent->numberOfChannels, 0);

	if (!textureComponent->rgbImage)
		return;
	//assert(textureComponent->rgbImage); // A bit harsh maybe. Implement graceful failing of image loading

	glGenTextures(1, &textureComponent->textureID);
	glBindTexture(GL_TEXTURE_2D, textureComponent->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureComponent->width, textureComponent->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureComponent->rgbImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureComponent->rgbImage);
}
