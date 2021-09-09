#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "gltypes.h"

struct TransformComponent
{
    int entity = 0;
    gsl::Matrix4x4 mMatrix;
};

struct MeshComponent
{
    int entity = 0;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLenum mDrawType{GL_TRIANGLES};
};

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
