#ifndef COMPONENTS_H
#define COMPONENTS_H

//Components try to hold only runtime relevant data, and as small as possible
#include "constants.h"

struct TransformComponent
{
    TransformComponent()
    {
        mMatrix.identity();
        mScale.setAlltoSame(1.f);
    };
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
    float mColliderRadius{0.f}; //used for frustum culling, so it is relevant for rendering
};

struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};  //this holds standard texture
    gsl::Vector3D mColor{0.4f};
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

struct LightComponet
{
//    gsl::Vector3D mPosition{10.f, 10.f, -5.f};    //given in the game object
    gsl::LightTypes mType{gsl::POINT};      //only POINT is supported for now
    gsl::Vector3D mDirection{0.f, -1.f, 0.f};   //not used in POINT
    gsl::Vector3D mColor{0.9f};
    float mLightStrenght{1.0f};
    gsl::Vector3D mAmbientColor{0.5f};
    float mAmbientStrength{0.5f};
    float mSpecularStrength{0.3f};
    short mSpecularExponent{32};
};

#endif // COMPONENTS_H
