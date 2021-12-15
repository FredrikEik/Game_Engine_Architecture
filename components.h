#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "vector4d.h"
#include "gltypes.h"

struct TransformComponent
{
public:
    TransformComponent();
    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent
{
public:
    MeshComponent();
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLenum mDrawType{GL_TRIANGLES};
    GLint mMatrixUniform{0};
};

class MaterialComponent
{
public:
    MaterialComponent();
    GLuint mShaderProgram;
    GLint mTextureUnit;



    void setupUniforms();


};

struct LightComponent
{
    gsl::Vector3D lightColor();
};

struct BoxCollisionComponent
{
public:
    gsl::Vector3D min, max;

};

struct SphereCollisionComponent
{
public:
    gsl::Vector3D center;
    float radius;
};

struct FrustumCollisionComponent
{
public:
    // near plane
    float nearPlaneLength;
    gsl::Vector3D leftBottomNear;
    gsl::Vector3D rightBottomNear;
    gsl::Vector3D leftTopNear;
    gsl::Vector3D rightTopNear;

    // far plane
    float farPlaneLength;
    gsl::Vector3D leftBottomFar;
    gsl::Vector3D rightBottomFar;
    gsl::Vector3D leftTopFar;
    gsl::Vector3D rightTopFar;

    gsl::Matrix4x4 mMatrix;
};

struct InputComponent
{
public:
    bool W{false};
    bool A{false};
    bool S{false};
    bool D{false};
    bool UP{false};     //Arrow keys
    bool DOWN{false};
    bool LEFT{false};
    bool RIGHT{false};
    bool Q{false};
    bool E{false};
    bool C{false};
    bool LSHIFT{false};
    bool LCTRL{false};
    bool SPACE{false};
    bool LMB{false};    //Mouse buttons
    bool RMB{false};
    bool MMB{false};
    float MWHEEL{0.f};  //MouseWheel
    int MOUSEX{0};
    int MOUSEY{0};

};

struct ParticleComponent
{
    struct Particle{
        gsl::Vector4D startColor{1,0,0,1}, endColor{1,0,0,0};
        gsl::Vector3D position{};
        gsl::Vector3D velocity{};
        gsl::Vector3D acceleration{};
        float speed{};
        float lifeSpan{};
        float lifeLeft{};
        float timeAlive{};
        float cameraDistance{-1.f };
        float startSize{}, endSize{};
        bool active{ false };
        bool operator<(const Particle& other)
        {
            return (this->active && this->cameraDistance > other.cameraDistance) || (this->active && !other.active);
        }

    };


    struct ParticleBlueprint
    {
        Particle particle;
        gsl::Vector3D positionMinOffset{0,0,0}, positionMaxOffset{1,0.1,1};
        gsl::Vector3D velocityMinOffset{0,0,0}, velocityMaxOffset{0,0,0};
        gsl::Vector3D accelerationMinOffset{0.00,0.00,0.00}, accelerationMaxOffset{0.00,0.005,0.00};
        gsl::Vector4D colorMinOffset{0,0,0,0}, colorMaxOffset{1,0.2,1,1};
        float sizeMinOffset{0.1}, sizeMaxOffset{0.3};
        float lifeMinOffset{20}, lifeMaxOffset{30};
    };

    MeshComponent mesh;
    MaterialComponent material;
    std::vector<Particle> particles;
    std::vector<float> positionData;
    std::vector<float> colorData;
    std::vector<float> lifeSizeData;
    int maxParticles{};
    int activeParticles{};
    int lastParticle{};
    int spawnRate{};
    float timeSinceLastSpawn{};
    float emitterLifeSpan{};

    ParticleBlueprint particleBlueprint;

    GLuint positionBuffer;
    GLuint colorBuffer;
    GLuint lifeSizeBuffer;

};

#endif // COMPONENTS_H
