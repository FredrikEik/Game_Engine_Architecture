#include "collisionsystem.h"

void CollisionComponent::setBoundingBox(gsl::Vector3D Min, gsl::Vector3D Max)
{
    BoundingBoxMin = Min;
    BoundingBoxMax = Max;
}

void CollisionComponent::moveBoundingBox(float dx, float dy, float dz)
{
    BoundingBoxMin.setX(BoundingBoxMin.x + dx);
    BoundingBoxMin.setY(BoundingBoxMin.y + dy);
    BoundingBoxMin.setZ(BoundingBoxMin.z + dz);
    BoundingBoxMax.setX(BoundingBoxMax.x + dx);
    BoundingBoxMax.setY(BoundingBoxMax.y + dy);
    BoundingBoxMax.setZ(BoundingBoxMax.z + dz);
}

void CollisionComponent::setBoundingSphere(float r, gsl::Vector3D pos)
{
    radius = r;
    center = pos;
}

void CollisionComponent::moveBoundingSphere(float dx, float dy, float dz)
{
    center.setX(center.x + dx);
    center.setY(center.y + dy);
    center.setZ(center.z + dz);
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
                (aCollision->center.getY()- bCollision->center.getY())*(aCollision->center.getY() - bCollision->center.getY())
                );
    return distance < (aCollision->radius + bCollision->radius);
}
