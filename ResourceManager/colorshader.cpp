#include "colorshader.h"
#include "camera.h"

/********************************************//**
* ... part of the shader system i worked on for the engine
***********************************************/

colorshader::colorshader(const std::string &shaderName)  : ShaderHandler(shaderName)
{

}
void colorshader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );
}
void colorshader::use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material *)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}
