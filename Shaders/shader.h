#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    // Use the current shader
    virtual void use();

    //setup for the shader
    virtual void setup() = 0;

    //Get program number for this shader
    GLuint getProgram() const;
    GLuint getModelMatrixUniform() const;
    GLint getProjectionMatrixUniform() const;
    GLint getViewMatrixUniform() const;

    GLint getLightColorUniform() const;
    GLint getLightPositionUniform() const;
    GLint getAmbientStrengthUniform() const;
    GLint getLightStrengthUniform() const;
    GLint getObjectColorUniform() const;
    GLint getSpecularStrengthUniform() const;
    GLint getSpecularExponentUniform() const;
    GLint getObjectAlphaUniform() const;
    GLint getTextureUniform() const;

protected:
    GLint m_modelMatrixUniform{-1};
    GLint m_projectionMatrixUniform{-1};
    GLint m_viewMatrixUniform{-1};

    GLint m_ambientStrengthUniform{-1};
    GLint m_lightStrengthUniform{-1};
    GLint m_lightColorUniform{-1};
    GLint m_lightPositionUniform{-1};

    GLint m_objectColorUniform{-1};
    GLint m_cameraPositionUniform{-1};

    GLint m_specularStrengthUniform{-1};
    GLint m_specularExponentUniform{-1};

    GLint m_objectAlphaUniform{-1};

    GLint m_textureUniform{-1};

private:
    GLuint m_program;
};

#endif
