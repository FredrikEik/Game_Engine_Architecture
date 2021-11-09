#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "altypes.h"

namespace ecs {

struct TransformComponent
{
    TransformComponent() {mMatrix.identity();};
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D position{0, 0, 0};
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
struct MaterialID
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    unsigned short mMaterialIndex{0};        //Material 0 = default material - should always exist!
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
struct Script
{
    std::string mName;

    //player name
    QString pName;
    int pLevel;
    double pHp;
};

} //namespace ends

#endif // COMPONENTS_H
