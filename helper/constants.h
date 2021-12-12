#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include "gltypes.h"

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA2021/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string ModelFilePath{AssetFilePath + "Models/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};

//Joinked from Ole.Experiment - start
    // Mathematical constants
    constexpr double PI_D = 3.141592653589793238462643383279502884;
    constexpr float PI = 3.14159265358979323846264338f;
    constexpr float E = 2.71828182845904523536f;
    constexpr float GRAVITY = 9.80665f; // Differs depending on where you are on the earth, ~9.81

    // Functions
    inline GLdouble rad2deg(GLdouble rad)
    {return rad * (180.0 / PI_D);}

    inline GLdouble deg2rad(GLdouble deg)
    {return deg * (PI_D / 180.0);}

    inline GLfloat rad2degf(GLfloat rad)
    {return rad * (180.0f / PI);}

    inline GLfloat deg2radf(GLfloat deg)
    {return deg * (PI / 180.0f);}
//Taken from Ole.Experiments - end
    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER
    };

    const int NumberOfShaders{4};
    const int NumberOfTextures{16};
    const int NumberOfMaterials{10};

    enum Components
    {
        NONE = -1,
        TRANSFORM = 0,
        MESH,
        MATERIAL
    };
}
#endif // CONSTANTS_H
