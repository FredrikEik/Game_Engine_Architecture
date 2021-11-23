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
    bool isDivisibleBy5(std::string str)
    {
        int n = str.length();

        return ( ((str[n-1]-'0') == 0) ||
                 ((str[n-1]-'0') == 5));
    }
    std::vector<Vertex> getCountourPoints() const;
    void setCountourPoints(const std::vector<Vertex> &value);

private:
    void ReadDatafromFile(std::string fileName);
    std::vector<Vertex> positions;
    std::vector<Vertex> mVertices;
    std::vector<Vertex> CountourPoints;
    std::pair<int,Vertex> CountourLines;
    void populatePosArr();

    float PosArr[1000][2000];
    void RemoveDeltaPos();
    void GenerateHeightMap();
    float CalcHeight(float x = 0, float z = 0);


    float heighestY = 0;
    float lowestY = 0;

};

#endif // LASHEIGHTMAP_H
