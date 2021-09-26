#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "vertex.h"
#include "matrix4x4.h"
#include "gltypes.h"

#include <al.h>
#include <alc.h>


class Copmonents
{
public:
    Copmonents();
};

class TransformComponent
{
public:
//    TransformComponent();
    gsl::Matrix4x4 mMatrix;
};

class MeshComponent
{
public:
//    MeshComponent();
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];

    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};

    GLenum mDrawType{GL_TRIANGLES};
    bool bUsingLOD{false};
};

class MaterialComponent
{
public:
//    MaterialComponent();
    GLuint mShaderProgram{};
    GLint mTextureUnit{};
};

class SoundListenerComponent
{
public:
    //should be fetced from other component?
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

class SoundSourceComponent
{
public:
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    bool looping;
    float gain;
};


//class ShaderComponent
//{
//public:
//    GLint mMatrixUniform{0};
//    GLint vMatrixUniform{0};
//    GLint pMatrixUniform{0};
//    GLint TextureUniform{0};
//};

//// TO DO: Create multiple shadercomponents, plainshadercomp, textureshadercomp...
//class PlainShaderComponent
//{
//public:
//    GLint mMatrixUniform{0};
//    GLint vMatrixUniform{0};
//    GLint pMatrixUniform{0};
//};

//class TextureShaderComponent
//{
//public:
//    GLint mMatrixUniform{0};
//    GLint vMatrixUniform{0};
//    GLint pMatrixUniform{0};
//    GLint TextureUniform{0};
//};



//struct TransformComponent
//{
//    gsl::Matrix4x4 mMatrix;
//};

//struct MeshComponent
//{
//    GLuint mVAO{0};
//    GLuint mVBO{0};
//    GLuint mEAB{0};
//    std::vector<Vertex> mVertices;
//    std::vector<GLuint> mIndices;
//    GLenum mDrawType{GL_TRIANGLES};
//};

//struct MaterialComponent
//{
//    GLuint mShaderProgram{0};
//    GLint mTextureUnit{0};
//};

#endif // COMPONENTS_H
