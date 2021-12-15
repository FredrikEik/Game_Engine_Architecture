#ifndef LASSURFACE_H
#define LASSURFACE_H
#include "gameobject.h"

#pragma once
///
/// \brief A class for reading LAS heightdata and triangulating to mesh
///
class TriangleSurface : public GameObject
{
public:
    TriangleSurface(std::string fileName);
    ~TriangleSurface() override;

    void readLAS(std::string fileName);
    void minMaxNormalize();
    void makeContourLines();


    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

    std::vector<Vertex> contourPoints;
private:

    std::vector<Vertex> lasData;

    int getState(bool A, bool B, bool C, bool D);

    //mins and maxes must be uniform with step
    float xMin{-40.f}, yMin{0.f}, zMin{-40.f}, xMax{40.f}, yMax{1000.f}, zMax{40.0f};
    float step{2.f};

    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
    //int maxVertices{1000};
};

#endif // LASSURFACE_H
