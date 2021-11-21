#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"

struct MeshData
{
    //according to https://www.learncpp.com/cpp-tutorial/arrays-part-ii/
    //an empty {} should initialize all elements to 0
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

    //Does not work, altough i want to return mVertices of the gameobject calling it.
    std::vector<Vertex> get_MeshData_mVertices() { return mVertices[0]; } //Only need the first, beacuse there is no LOD.
};


class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    MeshData makeLineBox(std::string meshName);

    MeshData makeCircleSphere(float radius = 1, bool rgbColor = true);

    MeshData makeFrustum(const struct Frustum &frustumIn);

    ///Makes a line for debug drawing.
    /// positionIn is origin
    MeshData makeLine(gsl::Vector3D &positionIn, int direction = 1, float lenght = 1.f, gsl::Vector3D colorIn = {1.f, 0.301f, 0.933f});

    ///Makes a line for debug drawing.
    MeshData makeLine(gsl::Vector3D &startIn, gsl::Vector3D endIn, float lenght = 1.f, gsl::Vector3D colorIn = {1.f, 0.301f, 0.933f});

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;

    std::vector<MeshData> get_mMeshes() { return mMeshes; }
    std::map<std::string, unsigned int> get_mMeshMap() { return mMeshMap; }

    void readLasFile();

private:
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    int makeEditorGrid(int size = 10, int scale = 1);

    void makeColliderCorners(MeshData &meshIn, gsl::Vector3D &vertexIn);

    //initalizes the mesh with OpenGL buffers - VAO, VBO, EAB
    void initMesh(MeshData &currentMesh, int lodLevel);

    class Logger* mLogger{nullptr};
};

#endif // MESHHANDLER_H
