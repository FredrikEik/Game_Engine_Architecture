#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

#define MAX_ENTITIES 65535
#define MAX_ENTITIES_TYPE unsigned short    //have to hold MAX_ENTITIES !!!
#define MAX_COMPONENTS 100                  //could be made dynamic by using std::vectors

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string MeshFilePath{AssetFilePath + "Meshes/"};
   // const std::string MeshFilePath{AssetFilePath + "C:/Users/fes22/Documents/GitHub/GEA2021/Assets/Meshes/"};
    const std::string SoundFilePath{AssetFilePath + "Sounds/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};
    const std::string ScriptFilePath{AssetFilePath + "Scripts/"};

    const int MAX_MESHCOMPONENTS = 1024;    //used in ResourceFactory - this is a hack - change this!

    const int NumberOfShaders{6};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER,
        MOUSEPICKSHADER,
        PARTICLESHADER,
        NOSHADER = 65535
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
        WAV = 4,
        JS = 5
    };
}
#endif // CONSTANTS_H
