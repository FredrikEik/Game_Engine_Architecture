#ifndef OBJMESH_H
#define OBJMESH_H

#include "vertex.h"
//#include "material.h"

#include <string>
#include <vector>
#include <visualobject.h>
#include <QMatrix4x4>


class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename);
    ~ObjMesh() override;

    void draw() override;
    void init() override;

    void readFile(std::string filename);

private:


};

#endif // OBJMESH_H
