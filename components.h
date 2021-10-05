#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>
#include "constants.h"
#include "matrix4x4.h"
#include "gltypes.h"
#include "vertex.h"
#include "altypes.h"

struct TransformComponent
{
    TransformComponent(){}
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D Rot;
    gsl::Vector3D Scal = gsl::Vector3D(1.f, 1.f, 1.f);
};

struct MeshComponent
{
    MeshComponent(){}

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLint mVertexCount[3]{-1};
    GLint mIndexCount[3]{-1};
    GLenum mDrawType{GL_TRIANGLES};
    float mColliderRadius{0.f};

    GLuint mVAO[3]{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLint mMatrixUniform{0};
};

struct MaterialComponent
{
    MaterialComponent(){}

    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};

    // Texture shader uniforms
    GLint mTextureUniform{-1};
};

struct CollisionComponent
{
    gsl::Vector3D min, max;
};

struct SoundComponent
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
