#include "transformsystem.h"
#include "gameobject.h"
#include "constants.h"

TransformSystem* TransformSystem::mInstance = nullptr;

TransformSystem::TransformSystem()
{

}

TransformSystem *TransformSystem::getInstance()
{
    if(!mInstance)
    {
        mInstance = new TransformSystem();
    }
    return mInstance;
}

void TransformSystem::setPosition(GameObject *obj, gsl::Vector3D moveToPos)
{
    obj->mTransformComp->mScaleMatrix.setPosition(moveToPos.x,moveToPos.y,moveToPos.z);

    updateMatrix(obj);
}

void TransformSystem::setScale(GameObject *obj, gsl::Vector3D scaleToValue)
{
    obj->mTransformComp->mScale = scaleToValue;
    updateMatrix(obj);
}

void TransformSystem::setRotation(GameObject *obj, gsl::Vector3D rotateToValue)
{
    float x = rotateToValue.x - obj->mTransformComp->mRotation.getX();
    float y = rotateToValue.y - obj->mTransformComp->mRotation.getY();
    float z = rotateToValue.z - obj->mTransformComp->mRotation.getZ();
    obj->mTransformComp->mRotation.setX(x+obj->mTransformComp->mRotation.getX());
    obj->mTransformComp->mRotation.setY(y+obj->mTransformComp->mRotation.getY());
    obj->mTransformComp->mRotation.setZ(z+obj->mTransformComp->mRotation.getZ());
    obj->mTransformComp->mScaleMatrix.rotateX(x);
    obj->mTransformComp->mScaleMatrix.rotateY(y);
    obj->mTransformComp->mScaleMatrix.rotateZ(z);

    float yrot = obj->mTransformComp->mRotation.y;
    if((yrot > 45 && yrot < 135) || ((yrot > 220 && yrot < 315)))
    {
        obj->mCollisionComp->bRotated = true;
        qDebug() << "maxcornen: " << obj->mCollisionComp->maxCorner << "mincorner: " << obj->mCollisionComp->minCorner;
        if(rotateOnce)
        {
            float tempMaxX = obj->mCollisionComp->maxCorner.x();
            obj->mCollisionComp->maxCorner.setX(obj->mCollisionComp->maxCorner.z());
            obj->mCollisionComp->maxCorner.setZ(tempMaxX);

            float tempMinX = obj->mCollisionComp->minCorner.x();
            obj->mCollisionComp->minCorner.setX(obj->mCollisionComp->minCorner.z());
            obj->mCollisionComp->minCorner.setZ(tempMinX);
            rotateOnce = false;
        }
    }
    else if((yrot < -45 && yrot > -135) || ((yrot < -220 && yrot > -315)))
    {
        obj->mCollisionComp->bRotated = true;
        qDebug() << "maxcornen: " << obj->mCollisionComp->maxCorner << "mincorner: " << obj->mCollisionComp->minCorner;
        if(rotateOnce)
        {
            float tempMaxX = obj->mCollisionComp->maxCorner.x();
            obj->mCollisionComp->maxCorner.setX(obj->mCollisionComp->maxCorner.z());
            obj->mCollisionComp->maxCorner.setZ(tempMaxX);

            float tempMinX = obj->mCollisionComp->minCorner.x();
            obj->mCollisionComp->minCorner.setX(obj->mCollisionComp->minCorner.z());
            obj->mCollisionComp->minCorner.setZ(tempMinX);
            rotateOnce = false;
        }
    }else
    {
        obj->mCollisionComp->bRotated = false;
        rotateOnce = true;
    }
        qDebug() << "maxcornen: " << obj->mCollisionComp->maxCorner << "mincorner: " << obj->mCollisionComp->minCorner;

    updateMatrix(obj);
}

void TransformSystem::setCollisionRotation(GameObject *obj, float deg)
{

    float y = deg - obj->mCollisionComp->mRotation.getY();


    obj->mCollisionComp->mRotation.setY(y+obj->mCollisionComp->mRotation.getY());


    obj->mCollisionComp->mScaleMatrix.rotateY(y);


    updateMatrix(obj);
}

void TransformSystem::updateMatrix(GameObject* obj)
{
    obj->mTransformComp->mMatrix = obj->mTransformComp->mScaleMatrix;
    obj->mTransformComp->mMatrix.scale(obj->mTransformComp->mScale);
}
