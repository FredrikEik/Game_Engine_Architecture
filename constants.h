#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string MeshFilePath{AssetFilePath + "Meshes/"};
    const std::string DataFilePath{AssetFilePath + "Datasett/"};
    const std::string SoundFilePath{AssetFilePath + "Sounds/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    const int MAX_MESHCOMPONENTS = 1024;

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
    };

    enum Components
    {
        NOCOMPONENTTYPE = -1,
        TRANSFORM = 0,
        MESH,
        MATERIAL
    };

    enum AssetType
    {
        NOASSETTYPE = 0,
        OBJ = 1,
        FBX = 2,
        BMP = 3,
        WAV = 4
    };
    enum LightTypes
    {
        POINT,  //Only this is supported for now
        SPOT,
        DIRECTIONAL
    };
}
#endif // CONSTANTS_H
