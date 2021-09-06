#include "textureshader.h"

TextureShader::TextureShader(const GLchar *vertexPath, const GLchar *fragmentPath)
    :Shader(vertexPath, fragmentPath)
{

}

void TextureShader::setup()
{
    m_modelMatrixUniform = glGetUniformLocation(getProgram(), "modelMatrix");
    m_viewMatrixUniform = glGetUniformLocation(getProgram(), "viewMatrix");
    m_projectionMatrixUniform = glGetUniformLocation(getProgram(), "projectionMatrix");
    m_textureUniform = glGetUniformLocation(getProgram(), "textureSampler");
}

