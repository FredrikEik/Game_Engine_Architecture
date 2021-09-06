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
    TransformComponent();
    gsl::Matrix4x4 mMatrix;
};

class MeshComponent
{
public:
    MeshComponent();
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

    GLenum mDrawType{GL_TRIANGLES};
};

class MaterialComponent
{
public:
    MaterialComponent();
    GLuint mShaderProgram{};
    GLint mTextureUnit{};
};

#endif // COMPONENTS_H
