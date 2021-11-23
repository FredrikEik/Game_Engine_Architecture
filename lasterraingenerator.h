#ifndef LASTERRAINGENERATOR_H
#define LASTERRAINGENERATOR_H

#include "gameobject.h"

class LasTerrainGenerator : public GameObject
{
public:
    LasTerrainGenerator(std::string fileName);
    ~LasTerrainGenerator();

    void draw();
    void init();
    void move(float x, float y, float z);
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
    float xMin{-20.f}, yMin{-10.f}, zMin{-20.f}, xMax{20.f}, yMax{20.f}, zMax{20.0f};
    float step{2.f};

    //int maxVertices{1000};
};

#endif // LASTERRAINGENERATOR_H
