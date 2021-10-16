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
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct Transform
{
    gsl::Matrix4x4 mMatrix;
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
//    gsl::Vector3D middlePoint{0,0,0};

    int lodLevel{0};

    GLenum mDrawType{GL_TRIANGLES};
};

struct SoundListener
{
    //should be fetced from other component?
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
