#ifndef CODEDMESHES_H
#define CODEDMESHES_H

#include <vector>
#include "vertex.h"


class CodedMeshes
{
public:
    CodedMeshes();
    void makeAll();
    void makeCubeVertices();
    void makeTriangleVertices();
    void makePlaneVertices();

    std::vector<Vertex> cubeVertices;
    std::vector<Vertex> triangleVertices;
    std::vector<Vertex> planeVertices;
};

#endif // CODEDMESHES_H
