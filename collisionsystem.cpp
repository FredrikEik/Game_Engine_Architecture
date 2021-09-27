#include "collisionsystem.h"

CollisionSystem::CollisionSystem()
{

}

void CollisionSystem::setBoundingBox(gsl::Vector3D Min, gsl::Vector3D Max)
{
    BoundingBoxMin = Min;
    BoundingBoxMax = Max;
}

void CollisionSystem::moveBoundingBox(float dx, float dy, float dz)
{
    BoundingBoxMin.setX(BoundingBoxMin.x + dx);
    BoundingBoxMin.setY(BoundingBoxMin.y + dy);
    BoundingBoxMin.setZ(BoundingBoxMin.z + dz);
    BoundingBoxMax.setX(BoundingBoxMax.x + dx);
    BoundingBoxMax.setY(BoundingBoxMax.y + dy);
    BoundingBoxMax.setZ(BoundingBoxMax.z + dz);
}
