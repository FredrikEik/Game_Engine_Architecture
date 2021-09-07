#ifndef COMPONENT_H
#define COMPONENT_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include <QOpenGLFunctions_4_1_Core>

struct TransformComponent
{
    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent
{
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLenum mDrawType{GL_TRIANGLES};
};

struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

#endif // COMPONENT_H
