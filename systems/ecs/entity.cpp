#include "entity.h"

//Entity::Entity(int id)
//{

//}

Entity::Entity()
{

    //setBoundingBox(mMesh->mVertices[0].get_xyz(),mMesh->mVertices[9].get_xyz());
}

Entity::~Entity()
{

}

void Entity::move(float x, float y, float z)
{
    // mTransform->mMatrix.getPosition() += mVelocity;
    //mTransform->mMatrix.setPosition(100.f, 10.f,0.f);
    mTransform->mMatrix.translate(x, y, z);
    BoundingBoxMin.x = BoundingBoxMin.x + x;
    BoundingBoxMin.y = BoundingBoxMin.y + y;
    BoundingBoxMin.z = BoundingBoxMin.z + z;
    BoundingBoxMax.x = BoundingBoxMax.x + x;
    BoundingBoxMax.y = BoundingBoxMax.y + y;
    BoundingBoxMax.z = BoundingBoxMax.z + z;

}

void Entity::setBoundingBox(gsl::Vector3D Min,gsl::Vector3D Max)
{
    BoundingBoxMin = Min;
    BoundingBoxMax = Max;
}
bool Entity::CheckCollide(gsl::Vector3D Pmin, gsl::Vector3D Pmax)
{
    bool check;

    if(BoundingBoxMax.x >= Pmin.x &&
            BoundingBoxMin.x <= Pmax.x &&
            BoundingBoxMax.y >= Pmin.y &&
            BoundingBoxMin.y <= Pmax.y &&
            BoundingBoxMax.z <= Pmin.z &&
            BoundingBoxMin.z >= Pmax.z)
    {
        //qDebug() << "collided ! ! ! ! ! ! ";
        check = true;
    }
    else
    {
        check = false;
    }

    return check;
}
void Entity::Collided()
{
    hit = true;
}

