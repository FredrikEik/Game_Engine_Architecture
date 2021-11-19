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

private:
    void ReadDatafromFile(std::string fileName);
    std::vector<Vertex> positions;
    void RemoveDeltaPos();
};

#endif // LASHEIGHTMAP_H
