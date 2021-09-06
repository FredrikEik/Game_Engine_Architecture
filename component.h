#ifndef COMPONENT_H
#define COMPONENT_H
#include <vector>

#include "vector3d.h"


class Component
{
public:

    virtual ~Component();

private:
    float x,y,z;
    gsl::Vector3D scale{x,y,z};
    gsl::Vector3D translate{x,y,z};
    gsl::Vector3D rotation{x,y,z};
};

#endif // COMPONENT_H
