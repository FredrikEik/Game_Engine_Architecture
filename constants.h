#ifndef CONSTANTS_H
#define CONSTANTS_H

#include<string>
/**
  Diverse konstanter for prosjektet.
  Blandt annet lokasjonen på hvor å finne relevante mapper i filstruktur og int-er for shaders, componenter og forhåndslagde ressurser.
 */
namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string MeshFilePath{AssetFilePath + "Meshes/"};
    const std::string SoundFilePath{AssetFilePath + "Sounds/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

    const std::string LODLevelPrefix{"_L0"};

    const int MAX_MESHCOMPONENTS = 1024;

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
    };

    enum Components
    {
        NONE = -1,
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
        POINT
    };

}
#endif // CONSTANTS_H
