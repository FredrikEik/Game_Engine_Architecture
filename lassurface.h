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
    void makeContourLines();


    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

    std::vector<Vertex> contourPoints;
private:

    std::vector<Vertex> lasData;

    int getState(bool A, bool B, bool C, bool D);

    //mins and maxes must be uniform with step
    float xMin{-30.f}, yMin{-5.f}, zMin{-30.f}, xMax{30.f}, yMax{15.f}, zMax{30.0f};
    float step{2.f};

    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
    //int maxVertices{1000};
};

#endif // LASSURFACE_H
