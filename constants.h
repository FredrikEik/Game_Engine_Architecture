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
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
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

    enum objectTypes
    {
        CUBE = 0,
        PLANE = 1,
        TRIANGLE = 2,
        MARIOCUBE = 3,
        SPHERE = 4,
        CAMERA = 5,
        PLAYER = 6,
        SKYBOX = 7,
        LIGHT = 8,
        LASSURFACE = 9,
        ROLLINGBALL = 10,
        CONTOURLINES = 11
    };
}
#endif // CONSTANTS_H
