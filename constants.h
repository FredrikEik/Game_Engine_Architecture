#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string MeshFilePath{AssetFilePath + "Meshes/"};
    const std::string HeightInfoPath{AssetFilePath + "HeightInfo/"};
    const std::string SoundFilePath{AssetFilePath + "Sounds/"};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER,
        SKYBOXSHADER
    };

    enum Components
        {
            NONE = 0,
            TRANSFORM,
            MESH,
            MATERIAL
        };

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};
}


#endif // CONSTANTS_H
