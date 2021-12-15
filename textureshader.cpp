#include "textureshader.h"
#include "camera.h"
#include "texture.h"

/********************************************//**
* ... part of the shader system i worked on for the engine
***********************************************/

textureshader::textureshader(const std::string &shaderName) : ShaderHandler(shaderName)
{

}
void textureshader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );
    mTextureUniform = glGetUniformLocation( mProgram, "textureSampler");
}
void textureshader::use(gsl::Matrix4x4 &modelMatrixIn, ecs::Material *materialIn)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());

    glUniform1i(mTextureUniform, materialIn->mTextureId-1);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}
