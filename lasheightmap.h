#ifndef LASHEIGHTMAP_H
#define LASHEIGHTMAP_H
#include <iostream>
#include <vector>
#include <QVector3D>
#include <fstream>
#include "vertex.h"

class LASHeightMap
{
private:
    static const int X =  800;
    static const int Z = 1300;
public:
    LASHeightMap(std::string fileName, int inResolution);

    std::vector<Vertex> getPositions() const;
    std::vector<Vertex> getmVertices() const;


    bool isDivisibleBy5(std::string str)
    {
        unsigned long long n = str.length();

        return ( ((str[n-1]-'0') == 0) ||
                 ((str[n-1]-'0') == 5));
    }
    std::vector<Vertex> getCountourPoints() const;
    void setCountourPoints(const std::vector<Vertex> &value);
    int getX(){return X;}
    int getZ(){return Z;}

public:
    //contooouurrr
    float mHPoints[X][Z];
    float getHeighestY(){return heighestY;}
private:
    int Resolution = 1; //1 is default and higest rez.
    float PosArr[1000][2000];
    float ContourPoints[50][1000];
    int ContourIndex[50];

    std::vector<Vertex> positions;
    std::vector<Vertex> mVertices;
    std::vector<Vertex> CountourPoints;
    std::pair<int,Vertex> CountourLines;

    void ReadDatafromFile(std::string fileName);
    void populatePosArr();

    void RemoveDeltaPos();
    void GenerateHeightMap();
    float CalcHeight(float x = 0, float z = 0);


    float heighestY = 0;
    float lowestY = 0;

};

#endif // LASHEIGHTMAP_H
