#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string SkyboxFilePath{AssetFilePath + "Skybox/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
    };

    enum AssetType
    {
        NULLASSETTYPE = 0,
        OBJ = 1,
        BMP = 2,
        WAV = 3
    };

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};
}
#endif // CONSTANTS_H
