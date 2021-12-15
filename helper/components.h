#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"

//! @enum objectType
//! helps us to difirentiate use cases of objects
enum objectType {
    helper = 0,
    skybox = 1,
    xyz = 2,
    lightSrc = 3,
    prop = 4,
    player = 5,
    enemy = 6
};

//! @struct DetailsComponent - holds information on components that we can display in editor
struct DetailsComponent{
    int entity = 0;
    std::string title;
    std::string description;
    objectType type = prop;
};

//! @struct Transformcomponent - Component holds transformation related data
//! @details Component contains data for translation, rotation Also contains physics data.
struct TransformComponent
{
    //! enityid
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

//!@struct MeshComponent contains data on mesh and render
//! @details meshComponent contains data on vertex, collision scale and helper boolians
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

    bool isDead = false;
    unsigned int LODLevel = 0;
    bool LODEnabled = false;

    GLenum mDrawType{GL_TRIANGLES};
};
//! @struct MaterialComponent contains data on material
struct MaterialComponent
{
    int entity = 0;
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
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
