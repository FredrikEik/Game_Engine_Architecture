#include "codedmeshes.h"

CodedMeshes::CodedMeshes()
{

}
void CodedMeshes::makeAll()
{
    makeCubeVertices();
    makeTriangleVertices();
    makePlaneVertices();
}
void CodedMeshes::makeCubeVertices()
{
    cubeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    cubeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    cubeVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    cubeVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    cubeVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    cubeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    cubeVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    cubeVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    cubeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
}

void CodedMeshes::makeTriangleVertices()
{
    triangleVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.f}); // Bottom Left
    triangleVertices.push_back(Vertex{ 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.f}); // Bottom Right
    triangleVertices.push_back(Vertex{ 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
}

void CodedMeshes::makePlaneVertices()
{
    planeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    planeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    planeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    planeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    planeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    planeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
}
