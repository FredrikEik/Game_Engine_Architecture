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
    gsl::Vector3D mPosition;
    float scale = 1;
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

struct NameComponent{
    int ObjectID = 0;
    std::string ObjectName;
     bool drawMe{true};

};


struct MovementComponent
{
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    float mPlayerSpeed{0.05f};

    int mMouseXlast{0};
    int mMouseYlast{0};

    int EposX{0};
    int EposZ{0};

    int PposX{0};
    int PposZ{0};
};


#endif // COMPONENT_H
