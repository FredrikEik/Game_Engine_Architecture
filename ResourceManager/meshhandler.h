#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"

struct MeshData
{
    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};
    GLint mVertexCount[3]{-1};
    GLint mIndexCount[3]{-1};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];
    //AABB Collider box: (the largest of these will be the radius for a Bounding sphere)
    gsl::Vector3D mUpRightFrontCorner{};
    gsl::Vector3D mLowLeftBackCorner{};
};

//Because we need OpenGLFunctions, this class can not be static
class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    MeshData makeLineBox(std::string meshName);

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;
private:
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();
    void makeColliderCorners(MeshData &meshIn, gsl::Vector3D &vertexIn);

    void initMesh(MeshData &tempMesh, int lodLevel);
};

#endif // MESHHANDLER_H
