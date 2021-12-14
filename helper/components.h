#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"


enum objectType {
    helper = 0,
    skybox = 1,
    xyz = 2,
    lightSrc = 3,
    prop = 4,
    player = 5,
    enemy = 6
};


struct DetailsComponent{
    int entity = 0;
    std::string title;

    std::string description;
    objectType type = prop;
};


struct TransformComponent
{
    int entity = 0;
    gsl::Matrix4x4 mMatrix;
    bool isPhysicsEnabled = false;

    gsl::Vector3D Velocity = gsl::Vector3D(0.0f,0.0f,0.0f);
    std::vector<gsl::Vector3D> PosOverTime;
    int LastTriangeindex =-1;
    bool bBSExists = false;

    gsl::Vector3D rotationTracker = gsl::Vector3D(0.0f,0.0f,0.0f);

    //bullet logic
    bool isBulletFired;
    bool isBulletLoaded;
};

struct MeshComponent
{
    int entity{0};

    std::vector<Vertex> mVertices; // Used for simple helper objects..

    bool IsRay = false;
    bool IsCollidable = true;

    GLuint mVAO[3] = {0,0,0};
    GLuint mVBO[3] = {0,0,0};
    GLuint mEAB[3] = {0,0,0};

    std::string meshName;
    size_t VertexSize[3] = {0,0,0};
    float collisionRadius = 0;
    QVector3D centerOfMesh = QVector3D(0.f,0.f,0.f);
    float scale = 1;

    bool isDrawable = true;
    bool isSceneObject = false;

    unsigned int LODLevel = 0;
    bool LODEnabled = false;

    GLenum mDrawType{GL_TRIANGLES};
};

struct MaterialComponent
{
    int entity = 0;
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

struct PhysicsComponent
{
    int entity = 0;
    float g = 9.81f;
    QVector3D GravityVec = QVector3D(0.f,-g,0.f);
    QVector3D VelocityVec = QVector3D(0.0f,0.0f,0.0f);
};

#endif // COMPONENTS_H

/*
MeshFactory->GetMesh("MeshName.obj")


createMeshComponentForEntity(int EntityId)
{
    Mesh x = MeshFactory->GetMesh("MESH.mesh")
    MeshComponents[enTityid] = new MeshComponent(x)


}


tick()
{
    per each meshcompoonet
            meshSystem->Render(meshComponent)
}


array MeshComponents -> rendre et mesh
for()
{

}




array Actor
    MeshComponent
    Dingledang
    QHashIterator
    Møkk
*/
