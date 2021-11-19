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
    void setPositions(const std::vector<Vertex> &value);

private:
    void ReadDatafromFile(std::string fileName);
    std::vector<Vertex> positions;
    void RemoveDeltaPos();
};

#endif // LASHEIGHTMAP_H
