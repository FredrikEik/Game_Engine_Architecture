#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class ShaderHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    ShaderHandler(const GLchar *vertexPath, const GLchar *fragmentPath);

    void setupShader(bool useTexture);

    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};
    GLint mTextureUniform{-1};

    ///OpenGL program ID for this shader
    GLuint mProgram;
    std::string mName{"none"};

private:
    class Logger* mLogger{nullptr};
};

#endif // SHADERHANDLER_H
