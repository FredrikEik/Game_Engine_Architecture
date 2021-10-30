#ifndef OBJREADER_H
#define OBJREADER_H

#include <iostream>
#include <vector>
#include "vertex.h"

class objReader
{
public:
    objReader();
    void readFile(std::string filename, std::vector<Vertex> *mVertices, std::vector<GLuint> *mIndices);
};

#endif // OBJREADER_H
