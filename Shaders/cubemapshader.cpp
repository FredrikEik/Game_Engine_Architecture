#include "cubemapshader.h"

CubemapShader::CubemapShader(const GLchar *vertexPath, const GLchar *fragmentPath)
    :Shader(vertexPath, fragmentPath)
{

}

void CubemapShader::setup()
{

    m_viewMatrixUniform = glGetUniformLocation(getProgram(), "viewMatrix");
    m_projectionMatrixUniform = glGetUniformLocation(getProgram(), "projectionMatrix");

    m_textureUniform = glGetUniformLocation(getProgram(), "cubeMap");
}
