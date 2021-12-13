#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "meshhandler.h"

/**
    Componenter som skal holde informasjon i sanntid.
    Komponentene er lagt til på "GameObjects" for å legge til funksjonalitet
    Dette er en del av ECS systemet, legge forhåndslagde komponenter på spillobjeckter/entites
*/
struct TransformComponent
{
    TransformComponent()
    {
        mMatrix.identity();
        mScale.setAlltoSame(1.0f);
    }
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D mScale;
    gsl::Vector3D mRotation;
};

struct MeshComponent
{
    GLuint mVAO[3]{};
    GLint mVertexCount[3]{-1, -1, -1};
    GLint mIndexCount[3]{-1, -1, -1};
    GLenum mDrawType{GL_TRIANGLES};
    float mColliderRadius{0.f}; //Used for frustum culling, so relevant for rendering
};

struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
    gsl::Vector3D mColor{0.4f};
    float mSpecularStrength{0.0f};
    short mSpecularExponent{0};
    bool mUseColor{false};
};

struct SoundListenerComponent
{
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

struct SoundComponent
{
    ALuint mSource;             ///< The sound source ID.
//    ALuint mBuffer;           ///< The buffer the source should play.
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    float gain{1.0f};
    bool shouldPlay{false};
//    bool isPlaying{false};
    bool looping{false};

};

struct PhysicsComponent
{
    bool simulatePhysics{true};
    gsl::Vector3D velocity{0.0f, 0.0f, 0.0f};
    gsl::Vector3D gravity{0.f, 0.f, 9.8f};
    gsl::Matrix4x4 mMatrix;
    gsl::Matrix4x4 mPosition;
    //gsl::Matrix4x4 mRotation;
    gsl::Matrix4x4 mScale;
};

struct PointLightComponent
{
    gsl::LightTypes mType{gsl::POINT};
    gsl::Vector3D mColor{0.7f};
    float mLightStrength{1.0f};
    gsl::Vector3D mAmbientColor{0.3};
    float mAmbientStrength{0.7};
};


#endif // COMPONENTS_H
