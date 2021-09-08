#include "phongshader.h"

PhongShader::PhongShader(const GLchar *vertexPath, const GLchar *fragmentPath)
    :Shader(vertexPath, fragmentPath)
{

}

void PhongShader::setup()
{
    m_modelMatrixUniform = glGetUniformLocation( getProgram(), "modelMatrix" );
    m_viewMatrixUniform = glGetUniformLocation( getProgram(), "viewMatrix" );
    m_projectionMatrixUniform = glGetUniformLocation(getProgram(), "projectionMatrix" );

    m_ambientStrengthUniform = glGetUniformLocation(getProgram(), "ambientStrength");
    m_lightStrengthUniform = glGetUniformLocation(getProgram(), "lightStrength");
    m_lightColorUniform = glGetUniformLocation(getProgram(), "lightColor");
    m_objectColorUniform = glGetUniformLocation(getProgram(), "objectColor");
    m_cameraPositionUniform = glGetUniformLocation(getProgram(), "cameraPosition");

    m_specularStrengthUniform = glGetUniformLocation(getProgram(), "specularStrength");
    m_specularExponentUniform = glGetUniformLocation(getProgram(), "specularExponent");

    m_lightPositionUniform = glGetUniformLocation(getProgram(), "lightPosition");

    m_objectAlphaUniform = glGetUniformLocation(getProgram(), "objectAlpha");
}
