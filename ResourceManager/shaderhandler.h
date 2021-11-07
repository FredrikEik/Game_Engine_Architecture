#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class ShaderHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    ShaderHandler(const GLchar *vertexPath, const GLchar *fragmentPath);

    ///gets the uniforms from the shader
    void setupShader(bool useTexture = false, bool useLight = false);

    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    GLint mTextureUniform{-1};

    GLint mCameraPosition{-1};

    GLint mLightPosition{-1};

    GLint mLightDirection{-1};

    GLint mLightColor{-1};
    GLint mObjectColor{-1};

    GLint mAmbientColor{-1};
    GLint mAmbientStrengt{-1};
    GLint mLightStrengt{-1};
    GLint mSpecularStrength{-1};
    GLint mSpecularExponent{-1};

    ///OpenGL program ID for this shader
    GLuint mProgram;
    std::string mName{"none"};

private:
    class Logger* mLogger{nullptr};
};

#endif
