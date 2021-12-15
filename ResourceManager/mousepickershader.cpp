#include "mousepickershader.h"
#include "camera.h"

/********************************************//**
* ... part of the shader system i worked on for the engine
***********************************************/

mousepickershader::mousepickershader(const std::string &shaderName) : ShaderHandler(shaderName)
{
}
void mousepickershader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "vpMatrix");
    mPickerColor = glGetUniformLocation(mProgram, "pickingColor");
}
void mousepickershader::use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material *)
{
    glUseProgram(mProgram);
    gsl::Matrix4x4 temp = mCurrentCamera->mProjectionMatrix * mCurrentCamera->mViewMatrix;
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, temp.constData());
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}
