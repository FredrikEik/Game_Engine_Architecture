#ifndef LASHEIGHTMAP_H
#define LASHEIGHTMAP_H
#include <iostream>
#include <vector>
#include <QVector3D>
#include <fstream>
#include "vertex.h"

class LASHeightMap
{
public:
    LASHeightMap(std::string fileName);

    std::vector<Vertex> getPositions() const;
    std::vector<Vertex> getmVertices() const;
    void setPositions(const std::vector<Vertex> &value);

private:
    void ReadDatafromFile(std::string fileName);
    std::vector<Vertex> positions;
    std::vector<Vertex> mVertices;
    void populatePosArr();

    float PosArr[1000][2000];
    void RemoveDeltaPos();
    void GenerateHeightMap();
    float CalcHeight(float x = 0, float z = 0);
};

#endif // LASHEIGHTMAP_H
