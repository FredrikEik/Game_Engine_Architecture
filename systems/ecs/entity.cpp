#include "entity.h"

//Entity::Entity(int id)
//{

//}

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::move(float x, float y, float z)
{
    // mTransform->mMatrix.getPosition() += mVelocity;
    //mTransform->mMatrix.setPosition(100.f, 10.f,0.f);
    mTransform->mMatrix.translate(x, z, y);
}

