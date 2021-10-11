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
    float mColliderRadius{0};
};

//Because we need OpenGLFunctions, this class can not be static
class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    ///Makes an AABB box matching the size of the mesh given
    MeshData makeLineBox(std::string meshName);

    ///Makes a sphere of 3 circles with given radius.
    /// Color option for rgb or pink
    MeshData makeCircleSphere(float radius = 1, bool rgbColor = true);

    MeshData makeFrustum(const struct Frustum &frustumIn);

    ///Makes a line for debug drawing.
    /// positionIn is origin
    MeshData makeLine(gsl::Vector3D &positionIn, int direction = 1, float lenght = 1.f, gsl::Vector3D colorIn = {1.f, 0.301f, 0.933f});

    ///Makes a line for debug drawing.
    MeshData makeLine(gsl::Vector3D &startIn, gsl::Vector3D endIn, float lenght = 1.f, gsl::Vector3D colorIn = {1.f, 0.301f, 0.933f});

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;
private:
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    ///Function that takes in a vertex, and compare to existing MeshData values.
    /// Makes numbers for bounding box and bounding sphere.
    /// Has to be run for every vertex in the mesh.
    void makeColliderData(MeshData &meshIn, gsl::Vector3D &vertexIn);

    ///Initializes the mesh with OpenGL buffers - VAO, VBO, EAB.
    void initMesh(MeshData &tempMesh, int lodLevel);

    class Logger* mLogger{nullptr};
};

#endif // MESHHANDLER_H
