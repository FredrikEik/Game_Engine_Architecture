#ifndef LASTERRAINGENERATOR_H
#define LASTERRAINGENERATOR_H

#include "gameobject.h"

class LasTerrainGenerator : public GameObject
{
public:
    LasTerrainGenerator();
    ~LasTerrainGenerator();

    void draw();
    void init();
    void move(float x, float y, float z);
    void readFile(std::string filename);
    void minMaxNormalize();

    std::vector<Vertex> mPointsArray;

    float x,y,z;

    float xMax = 20;
    float yMax = 20;
    float zMax = 20;

    float xMin = -20;
    float yMin = -20;
    float zMin = -20;


    float highestX;
    float highestY;
    float highestZ;

    float lowestX;
    float lowestY;
    float lowestZ;
};

#endif // LASTERRAINGENERATOR_H
