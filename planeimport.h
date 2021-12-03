#ifndef PLANEIMPORT_H
#define PLANEIMPORT_H

#include "gameobject.h"

class PlaneImport : public GameObject
{
public:
    PlaneImport(std::string filename);
    ~PlaneImport() override;

    virtual void draw() override;
    virtual void init() override;

    void readFile(std::string filename);
    void minMaxNormalize();
    std::vector<Vertex> mPointsArray;

    float x,y,z;

    float maxPointX;
    float maxPointY;
    float maxPointZ;

    float minPointX;
    float minPointY;
    float minPointZ;

    std::vector<Vertex> lasData;

    //mins and maxes must be uniform with step
    float xMin{-30.f}, yMin{-5.f}, zMin{-30.f}, xMax{30.f}, yMax{15.f}, zMax{30.0f};
    float step{2.f};
};

#endif // PLANEIMPORT_H
