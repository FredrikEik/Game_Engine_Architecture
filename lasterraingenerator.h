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
    float xMin{-30.f}, yMin{-5.f}, zMin{-30.f}, xMax{30.f}, yMax{15.f}, zMax{30.0f};
    float step{2.f};

    //int maxVertices{1000};
};

class ContourLines : public GameObject
{
public:
    ContourLines(GameObject* terrain);
    ~ContourLines() {};
    void createContourLines(GameObject* terrainw);
    void draw();
    void init();
    void move(float x, float y, float z);

};

#endif // LASTERRAINGENERATOR_H
