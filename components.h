#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"
#include "altypes.h"
#include "vector3d.h"
#include "vector4d.h"

/********************************************//**
* ... ECS for all the component for the engine.
***********************************************/

namespace ecs {
//! @struct TransformComponent - component for transformation data
//! @details contains data for translation and position
struct TransformComponent
{
    TransformComponent() {mMatrix.identity();};
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D position{0, 0, 0};
};
//! @struct MeshComponent - contains data on mesh
//! @details contains data on vertex and collision
struct MeshComponent
{
    GLuint mVAO[3]{0};
    GLint mVertexCount[3]{-1};
    GLint mIndexCount[3]{-1};
    GLenum mDrawType{GL_TRIANGLES};
    float mColliderRadius{0.f};
};
//! @struct MaterialComponent - contains data on material
//! @details contains data on texture and shaderhandler/program
struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLuint mShaderhandler{gsl::NOSHADER};
    GLint mTextureUnit{0};
};
//! @struct MaterialID - contains data on material with entityid
struct MaterialID
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    unsigned short mMaterialIndex{0};        //Material 0 = default material - should always exist!
};
//! @struct Material - contains data on material
//! @details contains, materialid, shader, texture, color and name
struct Material
{
    unsigned short mMaterialID{65535};
    unsigned short mShaderhandler{gsl::NOSHADER};
    unsigned short mTextureId{0};
    unsigned int mSpecularExponent{0};
    GLfloat mSpecularStrenght{0.f};
    gsl::Vector3D mColor{};
    std::string mMatName;
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
    gsl::Vector3D mVelocity;    ///< Vector containing source velocity.
    float gain{1.0f};
    bool shouldPlay{false};
    bool looping{false};

};
//! @struct Script - contains data on script
//! @details contains data for name, health, strength and speed
struct Script
{
    std::string mName;

    //Name
    QString pName;
    //Health
    double pHp;
    //Strength
    double pStrength;
    //Speed
    double pSpeed;
};
//struct ParticleComponent
//{

//    struct Particle
//    {
//        gsl::Vector3D position{};
//        gsl::Vector3D acceleration{};
//        gsl::Vector4D color{};
//        gsl::Vector3D velocity{};

//        bool active{false};
//        float cameraDis{-1.f};
//        float speed;
//        float life;
//        float timeAlive;
//        bool operator<(const Particle& other)
//        {
//            return (this->active && this->cameraDis > other.cameraDis) ||(this->active && !other.active);
//        }
//    };
//    struct ParticlePrint
//    {
//        Particle particle;
//        gsl::Vector4D colorMin{0,0,0,0}, colorMax{1,1,1,1};
//        gsl::Vector3D positionMin{0,0,0}, positionMax{0,0,0};
//        gsl::Vector3D velocityMin{0,0,0}, velocityMax{0,0,0};
//        float sizeMin{0}, sizeMax{0.1};
//        float lifeMin{20}, lifeMax{20};
//        gsl::Vector3D accelerationMin{-0.005, -0.005, -0.005}, accelerationMax{0.005, 0.005, 0.005};
//    };
//    MeshComponent mesh;
//    std::vector<Particle> particles;
//    std::vector<float> positionData;
//    std::vector<float> colorData;
//    MaterialComponent material;
//    int spawnRate;
//    int activeParticles;
//    int maxParticles;
//    int lastParticle;
//    float timeSinceSpawn;
//    float emitterSpan;

//    ParticlePrint particlePrint;
//    GLuint positionBuffer;
//    GLuint colorBuffer;


//};

} //namespace ends

#endif // COMPONENTS_H
