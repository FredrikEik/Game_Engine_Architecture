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
class ShaderHandler : public QOpenGLFunctions_4_1_Core
{
public:
    ShaderHandler();

    void init();

    Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};

    void setupShaderUniformLocation(int shaderIndex);
    void sendDataToShader(GameObject* gameObject);

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
};

#endif // SHADERHANDLER_H
