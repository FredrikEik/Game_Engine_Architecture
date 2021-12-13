#include "shaderhandler.h"
#include "constants.h"
ShaderHandler::ShaderHandler()
{

}

void ShaderHandler::init()
{
    initializeOpenGLFunctions();
    mShaderPrograms[gsl::Shaders::PLAINSHADER] = new Shader((gsl::ShaderFilePath + "plainvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "plainfragment.frag").c_str());
                                    qDebug() << "Plain shader program id: " << mShaderPrograms[0]->getProgram();

    mShaderPrograms[gsl::Shaders::TEXTURESHADER] = new Shader((gsl::ShaderFilePath + "textureshader.vert").c_str(),
                                    (gsl::ShaderFilePath + "textureshader.frag").c_str());
                                     qDebug() << "Texture shader program id: " << mShaderPrograms[1]->getProgram();

    mShaderPrograms[gsl::Shaders::PHONGSHADER] = new Shader((gsl::ShaderFilePath + "lightShader.vert").c_str(),
                                    (gsl::ShaderFilePath + "lightShader.frag").c_str());
                                     qDebug() << "Light shader program id: " << mShaderPrograms[2]->getProgram();

    mShaderPrograms[gsl::Shaders::SKYBOXSHADER] = new Shader((gsl::ShaderFilePath + "skyboxvertex.vert").c_str(),
                                    (gsl::ShaderFilePath + "skyboxfragment.frag").c_str());
                                     qDebug() << "Skybox shader program id: " << mShaderPrograms[3]->getProgram();

                                     setupShaderUniformLocation(gsl::Shaders::PLAINSHADER);
                                     setupShaderUniformLocation(gsl::Shaders::TEXTURESHADER);
                                     setupShaderUniformLocation(gsl::Shaders::PHONGSHADER);
                                     setupShaderUniformLocation(gsl::Shaders::SKYBOXSHADER);
}

void ShaderHandler::setupShaderUniformLocation(int shaderIndex)
{
    switch(shaderIndex)
    {
    case gsl::Shaders::PLAINSHADER:
        mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
        vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
        pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
        break;
    case gsl::Shaders::TEXTURESHADER:
        mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
        vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
        pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
        mTextureUniform             = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
        break;
    case gsl::Shaders::PHONGSHADER:
        mMatrixUniform[shaderIndex]  = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
        vMatrixUniform[shaderIndex]  = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
        pMatrixUniform[shaderIndex]  = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
        mLightColorUniform           = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightColor" );
        mObjectColorUniform          = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "objectColor" );
        mAmbientLightStrengthUniform = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientStrength" );
        mAmbientColor                = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "ambientColor" );
        mLightPositionUniform        = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightPosition" );
        mSpecularStrengthUniform     = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularStrength" );
        mSpecularExponentUniform     = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "specularExponent" );
        mLightPowerUniform           = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "lightStrength" );
        mCameraPositionUniform       = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "cameraPosition" );
        mConstantUniform             = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "constant" );
        mLinearUniform               = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "linear" );
        mQuadraticUniform            = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "quadratic" );
        mPhongTextureUniform         = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "textureSampler");
        break;
    case gsl::Shaders::SKYBOXSHADER:
        mMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "mMatrix" );
        vMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "vMatrix" );
        pMatrixUniform[shaderIndex] = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "pMatrix" );
        mSkyboxUniform              = glGetUniformLocation( mShaderPrograms[shaderIndex]->getProgram(), "cubeSampler");
        break;
    default:
        break;
    }
}

void ShaderHandler::sendDataToShader(GameObject* gameObject)
{
    unsigned int shaderProgramIndex = gameObject->getMaterialComponent()->mShaderProgram;
    glUseProgram(mShaderPrograms[shaderProgramIndex]->getProgram()); // What shader program to use
    if(shaderProgramIndex == 1)
    {
        glUniform1i(mTextureUniform, gameObject->getMaterialComponent()->mTextureUnit);
    }
    if(shaderProgramIndex == 2)
    {
        for(unsigned int i = 0; i < lightRefs.size(); i++)
        {
            glUniform3f(mLightPositionUniform, lightRefs[i]->getTransformComponent()->mMatrix.getPosition().getX(), lightRefs[i]->getTransformComponent()->mMatrix.getPosition().getY(), lightRefs[i]->getTransformComponent()->mMatrix.getPosition().getZ());
            glUniform3f(mCameraPositionUniform, cameraRef->position().getX(), cameraRef->position().getY(), cameraRef->position().getZ());
            glUniform3f(mLightColorUniform, lightRefs[i]->mLightColor.getX(), lightRefs[i]->mLightColor.getY(), lightRefs[i]->mLightColor.getZ());
            glUniform1f(mSpecularStrengthUniform, lightRefs[i]->mSpecularStrength);
            glUniform1i(mSpecularExponentUniform, lightRefs[i]->mSpecularExponent);
            glUniform1f(mAmbientLightStrengthUniform, lightRefs[i]->mAmbientStrength);
            glUniform3f(mAmbientColor, lightRefs[i]->mAmbientColor.getX(), lightRefs[i]->mAmbientColor.getY(), lightRefs[i]->mAmbientColor.getZ());
            glUniform1f(mLightPowerUniform, lightRefs[i]->mLightStrength);
            glUniform1f(mConstantUniform, lightRefs[i]->constant);
            glUniform1f(mLinearUniform, lightRefs[i]->linear);
            glUniform1f(mQuadraticUniform, lightRefs[i]->quadratic);
            glUniform1i(mPhongTextureUniform, gameObject->getMaterialComponent()->mTextureUnit);
        }
    }
    if(shaderProgramIndex == 3)
    {
       glUniform1i(mSkyboxUniform, gameObject->getMaterialComponent()->mTextureUnit);
    }
    glUniformMatrix4fv( vMatrixUniform[shaderProgramIndex], 1, GL_TRUE, cameraRef->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform[shaderProgramIndex], 1, GL_TRUE, cameraRef->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform[shaderProgramIndex], 1, GL_TRUE, gameObject->getTransformComponent()->mMatrix.constData());
}
