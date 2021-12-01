#ifndef CODEDMESHES_H
#define CODEDMESHES_H


class CodedMeshes
{
public:
    CodedMeshes();

    void init();

    void createCubeVertices();
    void createPlaneVertices();
    void createTriangleVertices();
    void createXYZVertices();

    std::vector <Vertex> cubeVertices;
    std::vector <Vertex> planeVertices;
    std::vector <Vertex> triangleVertices;
    std::vector <Vertex> xyzVertices;
};

#endif // CODEDMESHES_H
