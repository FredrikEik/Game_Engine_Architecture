#ifndef LASSURFACE_H
#define LASSURFACE_H

#include "gameobject.h"
#include "vector3d.h"
#include "vertex.h"

class LasSurface : public GameObject
{
public:
    LasSurface(std::string filnavn);
    ~LasSurface() override;
    void readLasFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;
    void minMaxScale();
    std::vector<Vertex> mVertices;
    float xmin=-20.f, xmax=20.f, ymin=-20.f, ymax=20.f, zmin = 0.f, zmax =10.f, h=0.f;
    double xMinimum = 0, xMaximum = 1, yMinimum = 0, yMaximum=1, zMinimum = 0, zMaximum=1;
    int numberofPointsInside;
    Vertex vertex;
    Vertex lasVertex;
    std::vector<Vertex> tempVertices;
    std::vector<Vertex> lasVertices;

};

#endif // LASSURFACE_H
