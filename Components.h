#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "matrix4x4.h"
#include "vertex.h"
#include "constants.h"

struct Material
{
    Material();
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct Transform
{
    Transform();
    gsl::Matrix4x4 mMatrix;
};

struct Mesh
{
    Mesh();
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
};

#endif // COMPONENTS_H
