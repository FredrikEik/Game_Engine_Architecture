#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "matrix4x4.h"
#include "vertex.h"
#include "constants.h"
#include "gltypes.h"

#include <al.h>
#include <alc.h>
/**
 * The Material struct holds two variables:
 * - mShaderProgram (Decides what shader program the current object uses)
 * - mTextureUnit (holds the index to the texture that the current object uses)
 */
struct Material
{
    GLuint mShaderProgram{2};
    GLint mTextureUnit{0};
};

/**
 * The Transform struct holds two variables:
 * - mMatrix, the objects model matrix. (Is used to get position, rotation, and scale data).
 * - mForward, vector that points towards the objects 'local' forward direction. (starts off pointing towards -1 in z-direction).
 */
struct Transform
{
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D mForward = {0,0,-1};
};

/**
 * The Mesh struct holds all information related to an object's mesh.
 * This struct has 12 Variables:
 * - mVertices. Holds positions, normals, and UV coordinates for all Vertexes(points). There are 3 arrays of vertices because there is different vertex data for different LOD-levels.
 * - mIndices. Holds the order of which the vertices should be drawn. (For example, i use this when making a triangulation of the terrain).
 * - mMatrixUniform.
 * - mVAO. (Vertex Array Object).
 * - mVBO. (Vertex Buffer Object).
 * - mEAB. (Entity Array Buffer).
 * - sphereRadius. Holds radius of the objects collision-sphere.
 * - lodLevel. Holds the value of the objects current LOD-level. (1-3).
 * - collisionsEnabled. Bool that decides if the object should be collideable. (If the object should have a collisionSphere, and should be affected by other objects).
 * - renderObject. If false, the object will not render on screen. (Used in Frustum culling, objects outside of frustum has renderObject set to false).
 * - mDrawType. Holds the enum value of what draw type the current object should use. (e.g: GL_Triangles, GL_Lines, GL_Points, etc).
 */
struct Mesh
{
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];

    GLint mMatrixUniform{0};

    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};

    float sphereRadius{0};

    int lodLevel{0};
    bool bLodEnabled{true};

    bool collisionsEnabled{true};

    bool renderObject{false};

    GLenum mDrawType{GL_TRIANGLES};
};

/**
 * The Frustum struct holds three variables:
 * - FOV. Holds a float value which decides what degree the field of view (for the frustum culling) should be.
 * - farPlane. Holds a float value that decides how far away an object needs to be, to not be rendered.
 * - nearPlane. Holds a float value that decides how close to the camera an object can be, before it no longer gets rendered.
 */
struct Frustum
{
    float FOV = 45.f;
    float farPlane = 1000.f;
    float nearPlane = 0.5f;
};

/**
 * The SoundListener struct holds 3 variables:
 * - vel. Velocity.
 * - dir. Direction.
 * - up. Up-vector.
 */
struct SoundListener
{
    gsl::Vector3D vel{0.0f, 0.0f, 0.0f};
    gsl::Vector3D dir{0.0f, 0.0f, 1.0f};
    gsl::Vector3D up{0.0f, 1.0f, 0.0f};
};

/**
 * The SoundSourceComp struct holds 5 variables:
 * - mSource. Index of the sound/audio.
 * - mBuffer. ?
 * - mVelocity. Velocity vector of the soundSource.
 * - looping. Bool that decides if the sound should loop or not.
 * - gain. Float value that decides how loud the volume should be / how much the sound is amplified compared to the original.
 */
struct SoundSourceComp
{
    ALuint mSource;
    ALuint mBuffer;
    gsl::Vector3D mVelocity;
    bool looping;
    float gain;
};

#endif // COMPONENTS_H
