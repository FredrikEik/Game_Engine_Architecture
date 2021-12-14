#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/**
Constants - Holder nødvendige strings ig id'er for path og shaders.
 */

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
   // const std::string SkyBFilePath{AssetFilePath + TextureFilePath + "skybox/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
    };

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};
}
#endif // CONSTANTS_H
