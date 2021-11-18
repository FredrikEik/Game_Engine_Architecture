#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H


#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "gameobject.h"
#include "vector3d.h"

class TriangleSurface : public GameObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filnavn);
    ~TriangleSurface() override;
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;
    void construct();
    void construct_cylinder();
    void construct_plane();
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLint mMatrixUniform{0};
    gsl::Matrix4x4 mMatrix;
    float mapMinX{0};
    float mapMaxX{0};
    float mapMinY{0};
    float mapMaxY{0};
    std::vector<Vertex> mVertices;

     Vertex mMapTerrain;

    std::vector<Vertex>& get_vertices() { return mVertices; } // 191120
};

#endif // TRIANGLESURFACE_H
