#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "vertex.h"
#include "matrix4x4.h"
#include "gltypes.h"

#include <al.h>
#include <alc.h>


class Copmonents
{
public:
    Copmonents();
};

class TransformComponent
{
public:
//    TransformComponent();
    gsl::Matrix4x4 mMatrix;
//    gsl::Vector3D rotationDeg{0.f,0.f,0.f};
//    gsl::Vector3D scale{0.f,0.f,0.f};
};

class MeshComponent
{
public:
//    MeshComponent();
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];

    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};

    GLenum mDrawType{GL_TRIANGLES};
    bool bUsingLOD{false};
};

class MaterialComponent
{
public:
//    MaterialComponent();
    GLuint mShaderProgram{};
    GLint mTextureUnit{};
};

class CollisionComponent
{
public:

    QVector3D maxCorner{0,0,0};
    QVector3D minCorner{0,0,0};
//    float xMin{0};
//    float xMax{0};
//    float yMin{0};
//    float yMax{0};
//    float zMin{0};
//    float zMax{0};

    float mRaidus{0};

    QVector3D mVelocity{0,0,0};

    bool bShowCollisionBox{false};
};

//class BallPhysicsComponent
//{
//public:
//    float Radius{0};
//};

class SoundListenerComponent
{
public:
    //should be fetced from other component?
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

class SoundSourceComponent
{
public:
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    bool looping;
    float gain;
};


#endif // COMPONENTS_H
