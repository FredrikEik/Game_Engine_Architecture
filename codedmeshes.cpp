#include "codedmeshes.h"

CodedMeshes::CodedMeshes()
{

}

void CodedMeshes::init()
{
    createCubeVertices();
    createPlaneVertices();
    createTriangleVertices();
    createXYZVertices();
}
void CodedMeshes::createCubeVertices()
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

void CodedMeshes::createPlaneVertices()
{
    planeVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    planeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    planeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    planeVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    planeVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    planeVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
}

void CodedMeshes::createTriangleVertices()
{
    triangleVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.f}); // Bottom Left
    triangleVertices.push_back(Vertex{ 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.f}); // Bottom Right
    triangleVertices.push_back(Vertex{ 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
}

void CodedMeshes::createXYZVertices()
{
    xyzVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    xyzVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    xyzVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    xyzVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    xyzVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    xyzVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});
}
