#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"

struct MeshData
{
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    GLint mVertexCount{-1};
    GLint mIndexCount{-1};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
};

//Because we need OpenGLFunctions, this class can not be static
class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;
private:
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    void initMesh(MeshData &tempMesh);
};

#endif // MESHHANDLER_H