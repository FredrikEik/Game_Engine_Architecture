#ifndef OBJMESH_H
#define OBJMESH_H

#include "visualobject.h"

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename);
    ~ObjMesh();

    void init() override;

private:
    void readFile(std::string filename);
};

#endif // OBJMESH_H
