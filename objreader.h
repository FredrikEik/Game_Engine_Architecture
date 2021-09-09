#ifndef OBJREADER_H
#define OBJREADER_H

#include <iostream>
#include "components.h"

class ObjReader
{
public:

    ObjReader();

    void readFile(std::string filename, std::vector<Vertex> *mVertices, std::vector<GLuint> *mIndices);
private:

};


#endif // OBJREADER_H
