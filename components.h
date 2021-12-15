#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "altypes.h"

/*!
 * \brief ColliderComponent holder variablene for å sjekke sphere collision
 */

struct ColliderComponent
{
    bool objectsHasCollided = false;
    bool sphereCollider = false;
    bool OBBCollider = false;
    float distance{0};
    float radiusOfObjects{0};

};

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
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

struct LightComponet
{
    gsl::LightTypes mType{gsl::POINT};
    gsl::Vector3D mDirection{0.f, -1.f, 0.f};
    gsl::Vector3D mColor{0.9f};
    float mLightStrenght{1.0f};
    gsl::Vector3D mAmbientColor{0.5f};
    float mAmbientStrength{0.5f};
};

struct SoundComponet
{
    ALuint mSource;             ///< The sound source ID.
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    float gain{1};
    float setVolume(float vol)
    {
        gain = vol;

        return gain;
    };
    bool shouldPlay{false};
    bool looping{false};
    bool stop{false};

};

#endif // COMPONENTS_H
