#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "vector3d.h"
#include <math.h>

/**
*CollisionComponent holder raw data og funksjoner som setter verdier for collision.
*setBoundingBox - Setter min og max verdi av boundingbox.
*setBoundingSphere - Setter radius og posisjon til bounding sphere.
 */


struct CollisionComponent
{
    void setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max);
    void setBoundingSphere(float r, gsl::Vector3D pos);

    float radius;
    gsl::Vector3D center;
    gsl::Vector3D BoundingBoxMin;
    gsl::Vector3D BoundingBoxMax;

};


/**
*CollisionSystem - holder alle funksjoner for å regne ut collision.
*moveBoundingBox - Setter min og max verdi av boundingbox.
*moveBoundingSphere - Setter radius og posisjon til bounding sphere.
*CheckBoxCol - Sjekker kollisjon mellom box til box.
*CheckSphOnBoxCol - Sjekker kollisjon mellom sphere til box.
*CheckSphCol - Sjekker kollisjon mellom sphere til sphere.
*CheckMousePickCollision - Sjekker kollisjon mellom ray/distance og sphere.
 */

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
