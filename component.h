#ifndef COMPONENT_H
#define COMPONENT_H


#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "vector3d.h"
#include "gltypes.h"
#include <QOpenGLFunctions_4_1_Core>

struct TransformComponent
{
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D mPosition;
    float mScale = 1;
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

struct NameComponent
{
    int objectID=0;
    std::string mName;
};

struct MovementComponent
{
    gsl::Vector3D mForward{0,0,1};
    float mSpeed{0.05f};
    float mRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};
    int posX{0};
    int posZ{0};
};

#endif // COMPONENT_H
