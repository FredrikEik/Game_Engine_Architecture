#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <bitset>






namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    typedef uint16_t Entity;

    typedef uint8_t ComponentType;

    const Entity MAX_ENTITIES = 5000;
    const ComponentType MAX_COMPONENTS = 32;

    typedef std::bitset<MAX_COMPONENTS> Signature;

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
