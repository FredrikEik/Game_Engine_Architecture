#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include "shader.h"
#include "gameobject.h"
#include "light.h"
#include "camera.h"
class Shader;
class GameObject;
class Light;
class Camera;

///
/// \brief A class for handling shaders, setting up shaderuniforms and sending/retrieving data.
///
class ShaderHandler : public QOpenGLFunctions_4_1_Core
{
public:
    ShaderHandler();

    void init();

    ///Holds all shader programs
    Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};

    ///Sets up all uniformlocations
    void setupShaderUniformLocation(int shaderIndex);

    ///Retrieves data from gameObject and sends to shader
    void sendDataToShader(GameObject* gameObject);

    ///References to all lights in the scene.
    std::vector<Light*> lightRefs;

    Camera* cameraRef;
private:

    //MVP matrix uniforms
    GLint mMatrixUniform[4];
    GLint vMatrixUniform[4];
    GLint pMatrixUniform[4];

    // Skybox shader uniform
    GLint mSkyboxUniform;

    //Texture shader uniform
    GLint mTextureUniform;

    // Phong shader uniforms
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mAmbientColor{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mConstantUniform{-1};
    GLint mLinearUniform{-1};
    GLint mQuadraticUniform{-1};
    GLint mPhongTextureUniform{-1};

    // HUD shader uniform
    GLint mHUDUniform[4];
};

#endif // SHADERHANDLER_H
