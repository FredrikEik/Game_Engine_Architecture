#ifndef TERRAIN_H
#define TERRAIN_H

#include "gameobject.h"


class Terrain : public GameObject
{
public:
    Terrain(std::string fileName);
    ~Terrain();

    void draw() override;
    void init() override;
    void readFile(std::string filename);

    void minMaxNormalize();
    std::vector<Vertex> mPointsArray;

    float x,y,z;

    float highestX;
    float highestY;
    float highestZ;

    float lowestX;
    float lowestY;
    float lowestZ;

    std::vector<Vertex> lasData;

    //mins and maxes must be uniform with step
    float xMin{-30.f}, yMin{-5.f}, zMin{-30.f}, xMax{30.f}, yMax{15.f}, zMax{30.0f};
    float step{2.f};
};

#endif // TERRAIN_H
