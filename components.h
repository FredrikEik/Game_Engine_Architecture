#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "altypes.h"

struct TransformComponent
{
    TransformComponent() {mMatrix.identity();};
    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent
{
    GLuint mVAO[3]{0};
    GLint mVertexCount[3]{-1};
    GLint mIndexCount[3]{-1};
    GLenum mDrawType{GL_TRIANGLES};
    float mColliderRadius{0.f};
};

struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct SoundListenerComponent
{
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    //should be fetced from other component?
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    //should be fetced from other component?
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

struct SoundComponet
{
    ALuint mSource;             ///< The sound source ID.
//    ALuint mBuffer;             ///< The buffer the source should play.
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    float gain{1.0f};
    bool shouldPlay{false};
//    bool isPlaying{false};
    bool looping{false};

};

#endif // COMPONENTS_H
