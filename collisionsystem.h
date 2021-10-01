#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "vector3d.h"
#include <math.h>

class CollisionComponent
{
public:
    CollisionComponent(){};
    void setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max);
    void moveBoundingBox(float dx, float dy, float dz);
    void setBoundingSphere(float r, gsl::Vector3D pos);
    void moveBoundingSphere(float dx, float dy, float dz);

    float radius;
    gsl::Vector3D center;
    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;
};

class CollisionSystem : public CollisionComponent
{
public:
    CollisionSystem(){};
//    bool resultX;
//    bool resultY;
//    bool resultZ;
    bool CheckBoxCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
    bool CheckSphOnBoxCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
    bool CheckSphCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
};

#endif // COLLISIONSYSTEM_H
