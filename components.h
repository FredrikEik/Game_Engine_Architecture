#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "vertex.h"
#include "matrix4x4.h"
#include "gltypes.h"

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
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLenum mDrawType{GL_TRIANGLES};
};

class MaterialComponent
{
public:
//    MaterialComponent();
    GLuint mShaderProgram{};
    GLint mTextureUnit{};
};


class ShaderComponent
{
public:
    GLint mMatrixUniform{0};
    GLint vMatrixUniform{0};
    GLint pMatrixUniform{0};
    GLint TextureUniform{0};
};

// TO DO: Create multiple shadercomponents, plainshadercomp, textureshadercomp...
class PlainShaderComponent
{
public:
    GLint mMatrixUniform{0};
    GLint vMatrixUniform{0};
    GLint pMatrixUniform{0};
};

class TextureShaderComponent
{
public:
    GLint mMatrixUniform{0};
    GLint vMatrixUniform{0};
    GLint pMatrixUniform{0};
    GLint TextureUniform{0};
};



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
