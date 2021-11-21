#ifndef LASSURFACE_H
#define LASSURFACE_H
#include "gameobject.h"

#pragma once

class LASsurface : public GameObject
{
public:
    LASsurface(std::string fileName);
    ~LASsurface() override;

    void readLAS(std::string fileName);
    void minMaxNormalize();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

private:

    std::vector<Vertex> lasData;

    //mins and maxes must be uniform with step
    float xMin{-20.f}, yMin{0.f}, zMin{-20.f}, xMax{20.f}, yMax{6.f}, zMax{20.0f};
    float step{2.f};

    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
    //int maxVertices{1000};
};

#endif // LASSURFACE_H
