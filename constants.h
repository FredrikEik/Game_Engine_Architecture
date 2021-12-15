#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    enum Shaders
    {
        PLAINSHADER = 0,
        TEXTURESHADER = 1,
        PHONGSHADER = 2,
        SKYBOXSHADER = 3
    };

    enum ObjectType
    {
        CUBE = 0,
        PLANE = 1,
        TRIANGLE = 2,
        MARIOCUBE = 3,
        SPHERE = 4,
        CAMERA = 5,
        SKYBOX = 6,
        LIGHT = 7,
        PLAYER = 8,
        TRIANGLESURFACE = 9
    };

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};
    const std::uint32_t MAX_OBJECTS{5000};

    enum Components
    {
        NONE = -1,
        TRANSFORM = 0,
        MESH,
        MATERIAL
    };
}
#endif // CONSTANTS_H
