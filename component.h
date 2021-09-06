#ifndef COMPONENT_H
#define COMPONENT_H
#include "vector3d.h"

class Component
{
public:
    void update();
private:
    gsl::Vector3D position;
    gsl::Vector3D rotation;
    gsl::Vector3D scale;
};

#endif // COMPONENT_H
