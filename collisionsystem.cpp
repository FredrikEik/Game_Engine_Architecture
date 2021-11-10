#include "collisionsystem.h"

void CollisionComponent::setBoundingBox(gsl::Vector3D Min, gsl::Vector3D Max)
{
    BoundingBoxMin = Min;
    BoundingBoxMax = Max;
}

void CollisionSystem::moveBoundingBox(float dx, float dy, float dz, CollisionComponent *dCollision)
{
    dCollision->BoundingBoxMin.setX(dCollision->BoundingBoxMin.x + dx);
    dCollision->BoundingBoxMin.setY(dCollision->BoundingBoxMin.y + dy);
    dCollision->BoundingBoxMin.setZ(dCollision->BoundingBoxMin.z + dz);
    dCollision->BoundingBoxMax.setX(dCollision->BoundingBoxMax.x + dx);
    dCollision->BoundingBoxMax.setY(dCollision->BoundingBoxMax.y + dy);
    dCollision->BoundingBoxMax.setZ(dCollision->BoundingBoxMax.z + dz);
}

void CollisionComponent::setBoundingSphere(float r, gsl::Vector3D pos)
{
    radius = r;
    center = pos;
}

void CollisionSystem::moveBoundingSphere(float dx, float dy, float dz, CollisionComponent *dCollision)
{
    dCollision->center.setX(dCollision->center.x + dx);
    dCollision->center.setY(dCollision->center.y + dy);
    dCollision->center.setZ(dCollision->center.z + dz);
}

bool CollisionSystem::CheckBoxCol(CollisionComponent *aCollision, CollisionComponent *bCollision)
{

    //    bool result;
    //    //Fikse utregning her, dele opp if så den sjekker x y og z hver for seg
    //        if(aCollision->BoundingBoxMax.getX() >= bCollision->BoundingBoxMin.getX() && aCollision->BoundingBoxMin.getX() <= bCollision->BoundingBoxMax.getX())
    //             resultX = true;
    //        else
    //            resultX = false;

    //        if(aCollision->BoundingBoxMax.getY() >= bCollision->BoundingBoxMin.getY() && aCollision->BoundingBoxMin.getY() <= bCollision->BoundingBoxMax.getY())
    //             resultY = true;
    //        else
    //            resultY = false;

    //        if(aCollision->BoundingBoxMax.getZ() >= bCollision->BoundingBoxMin.getZ() && aCollision->BoundingBoxMin.getZ() <= bCollision->BoundingBoxMax.getZ())
    //             resultZ = true;
    //        else
    //            resultZ = false;

    //        if(resultX == true && resultY == true && resultZ == true)
    //            result = true;
    //        else result = false;
    //       // qDebug() << result;
    //        return result;

    bool result;
    if(aCollision->BoundingBoxMax.getX() >= bCollision->BoundingBoxMin.getX() && aCollision->BoundingBoxMin.getX() <= bCollision->BoundingBoxMax.getX()
            && aCollision->BoundingBoxMax.getY() >= bCollision->BoundingBoxMin.getY() && aCollision->BoundingBoxMin.getY() <= bCollision->BoundingBoxMax.getY()
            && aCollision->BoundingBoxMax.getZ() >= bCollision->BoundingBoxMin.getZ() && aCollision->BoundingBoxMin.getZ() <= bCollision->BoundingBoxMax.getZ())
        result = true;
    else result = false;
    // qDebug() << result;
    return result;
}

bool CollisionSystem::CheckSphOnBoxCol(CollisionComponent *sCollision, CollisionComponent *bCollision)
{
    // get box closest point to sphere center by clamping
    auto x = fmax(bCollision->BoundingBoxMin.getX(), fmin(sCollision->center.getX(), bCollision->BoundingBoxMax.getX()));
    auto y = fmax(bCollision->BoundingBoxMin.getY(), fmin(sCollision->center.getY(), bCollision->BoundingBoxMax.getY()));
    auto z = fmax(bCollision->BoundingBoxMin.getZ(), fmin(sCollision->center.getZ(), bCollision->BoundingBoxMax.getZ()));

    // this is the same as isPointInsideSphere
    auto distance = sqrt((x - sCollision->center.getX()) * (x - sCollision->center.getX()) +
                         (y - sCollision->center.getY()) * (y - sCollision->center.getY()) +
                         (z - sCollision->center.getZ()) * (z - sCollision->center.getZ()));

    return distance < sCollision->radius;
}

bool CollisionSystem::CheckSphCol(CollisionComponent *aCollision, CollisionComponent *bCollision)
{
    float distance = sqrt(
                (aCollision->center.getX()- bCollision->center.getX())*(aCollision->center.getX() - bCollision->center.getX())+
                (aCollision->center.getY()- bCollision->center.getY())*(aCollision->center.getY() - bCollision->center.getY())+
                (aCollision->center.getZ()- bCollision->center.getZ())*(aCollision->center.getZ() - bCollision->center.getZ())
                );
    return distance < (aCollision->radius + bCollision->radius);
}

bool CollisionSystem::CheckMousePickCollision(float distance, CollisionComponent *sCollision)
{
    //    float distance = sqrt((point.getX() - sCollision->center.getX()) * (point.x - sCollision->center.getX()) +
    //                          (point.y - sCollision->center.getY()) * (point.y - sCollision->center.getY()) +
    //                          (point.z - sCollision->center.getZ()) * (point.z - sCollision->center.getZ()));
    return distance < sCollision->radius;
}
