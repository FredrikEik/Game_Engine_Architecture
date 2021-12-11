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
    gsl::Vector3D curretPos = obj->mTransformComp->mScaleMatrix.getPosition();
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

    updateMatrix(obj);
}

void TransformSystem::updateMatrix(GameObject* obj)
{
    obj->mTransformComp->mMatrix = obj->mTransformComp->mScaleMatrix;
    obj->mTransformComp->mMatrix.scale(obj->mTransformComp->mScale);
}
