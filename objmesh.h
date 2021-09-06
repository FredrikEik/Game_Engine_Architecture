#ifndef OBJMESH_H
#define OBJMESH_H


#include "visualobject.h"


class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename);
    ~ObjMesh() override;

    virtual void draw() override;
    virtual void init() override;

    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;

    std::string filename;

    void setBoundingBox(gsl::Vector3D Min, gsl::Vector3D Max);

    bool CheckCollide(gsl::Vector3D Pmin, gsl::Vector3D Pmax);

    void reset();

    void readFile(std::string filename);
};

#endif // OBJMESH_H
