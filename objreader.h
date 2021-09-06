#ifndef OBJREADER_H
#define OBJREADER_H

#include <iostream>
#include "components.h"

class ObjReader
{
public:

    ObjReader();

    void readFile(std::string filename, MeshComponent &meshComp);
private:

};


#endif // OBJREADER_H
