#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>
#include "constants.h"
#include "matrix4x4.h"
#include "gltypes.h"
#include "vertex.h"

struct TransformComponent
{
    TransformComponent(){}
    gsl::Matrix4x4 mMatrix;
    gsl::Matrix4x4 mTrueScaleMatrix;
    gsl::Vector3D Rot;
    gsl::Vector3D Scal = gsl::Vector3D(1.f, 1.f, 1.f);
};

struct MeshComponent
{
    MeshComponent(){}

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLenum mDrawType{GL_TRIANGLES};
    GLint mMatrixUniform{0};
};

struct MaterialComponent
{
    MaterialComponent(){}

    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};

    // Texture shader uniforms
    GLint mTextureUniform{-1};
};

struct CollisionComponent
{
    gsl::Vector3D min = gsl::Vector3D(0.f);
    gsl::Vector3D max = gsl::Vector3D(0.f);
};


#endif // COMPONENTS_H
