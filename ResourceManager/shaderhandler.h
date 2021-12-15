#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class ShaderHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    ShaderHandler(const GLchar *vertexPath, const GLchar *fragmentPath);

    // Use the current shader
    void use( );
    void setupShader(bool useTexture = false, bool useLight = false); ///sets up shader


    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    std::string mName{"none"};

    //Get program number for this shader
    GLuint getProgram() const;

    GLuint program;
};

#endif
