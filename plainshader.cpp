#include "plainshader.h"

PlainShader::PlainShader(const GLchar *vertexPath, const GLchar *fragmentPath)
    :Shader(vertexPath, fragmentPath)
{

}

void PlainShader::setup()
{
    m_modelMatrixUniform = glGetUniformLocation(getProgram(), "modelMatrix");
    m_viewMatrixUniform = glGetUniformLocation(getProgram(), "viewMatrix");
    m_projectionMatrixUniform = glGetUniformLocation(getProgram(), "projectionMatrix");
}
