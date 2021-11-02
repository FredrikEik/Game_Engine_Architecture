#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"

struct MeshData
{
    GLuint mVAO[3]{};
    GLuint mVBO[3]{};
    GLuint mEAB[3]{};
    GLint mVertexCount[3]{-1, -1, -1};
    GLint mIndexCount[3]{-1, -1, -1};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];
    //AABB Collider box: (the largest of these will be the radius for a Bounding sphere)
    gsl::Vector3D mUpRightFrontCorner{};
    gsl::Vector3D mLowLeftBackCorner{};
    float mColliderRadius{0};
};

class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    MeshData makeLineBox(std::string meshName);

    MeshData makeCircleSphere(float radius = 1, bool rgbColor = true);

    MeshData makeFrustum(const struct Frustum &frustumIn);

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;

private:
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    void makeColliderCorners(MeshData &meshIn, gsl::Vector3D &vertexIn);

    //initalizes the mesh with OpenGL buffers - VAO, VBO, EAB
    void initMesh(MeshData &currentMesh, int lodLevel);

    class Logger* mLogger{nullptr};
};

#endif // MESHHANDLER_H
