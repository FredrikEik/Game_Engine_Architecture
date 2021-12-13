#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "vector3d.h"
#include <math.h>


struct CollisionComponent
{
    void setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max);
    void setBoundingSphere(float r, gsl::Vector3D pos);

    float radius;
    gsl::Vector3D center;
    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;

};

class CollisionSystem
{
public:
    CollisionSystem(){};
    ~CollisionSystem();
    void moveBoundingBox(float dx, float dy, float dz, CollisionComponent *dCollision);
    void moveBoundingSphere(float dx, float dy, float dz, CollisionComponent *dCollision);
    bool CheckBoxCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
    bool CheckSphOnBoxCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
    bool CheckSphCol(CollisionComponent *aCollision, CollisionComponent* bCollision);
    bool CheckMousePickCollision(CollisionComponent *aCollision, gsl::Vector3D objPos, gsl::Vector3D camPos, gsl::Vector3D ray);

};

#endif // COLLISIONSYSTEM_H
