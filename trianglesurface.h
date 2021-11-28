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

    gsl::Matrix4x4 mMatrix;
    float mapMinX{0};
    float mapMaxX{0};
    float mapMinY{0};
    float mapMaxY{0};
    float mapMinZ{0};
    float mapMaxZ{0};
    Vertex mMapTerrain;
};

#endif // TRIANGLESURFACE_H
