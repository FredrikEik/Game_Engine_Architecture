#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "matrix4x4.h"
#include "vertex.h"
#include "constants.h"
#include "gltypes.h"

#include <al.h>
#include <alc.h>

struct Material
{
    GLuint mShaderProgram{2};
    GLint mTextureUnit{0};
};

struct Transform
{
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D mForward = {0,0,-1};
};

struct Mesh
{
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];

    GLint mMatrixUniform{0};

    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};

    float sphereRadius{0};

    int lodLevel{0};
    bool bLodEnabled{true};

    bool collisionsEnabled{true};

    bool renderObject{false};

    GLenum mDrawType{GL_TRIANGLES};
};

struct Frustum
{
    float FOV = 45.f;
    float farPlane = 150.f;
    float nearPlane = 0.5f;
};

struct SoundListener
{
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

struct SoundSourceComp
{
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    bool looping;
    float gain;
};

#endif // COMPONENTS_H
