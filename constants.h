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

    enum Components{

        NONE = -1,
        TRANSFORM = 0,
        MESH,
        MATERIAL,
        COLLIDER
    };


    enum Colliders
    {
        NOCOLLIDER = -1,
        OBB,
        SPHERE
    };

}
#endif // CONSTANTS_H
