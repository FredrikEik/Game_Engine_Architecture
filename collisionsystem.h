#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "vector3d.h"

class CollisionSystem
{
public:
    CollisionSystem();

    void setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max);
    void moveBoundingBox(float dx, float dy, float dz);

private:
    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;


};

#endif // COLLISIONSYSTEM_H
