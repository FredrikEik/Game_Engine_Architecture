#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMoveComp = new MovementComponent;
    mTransform->mScale = 0.2;
    mMoveComp->mSpeed = 0.025;
    mMesh = factoryPtr->getMesh(5);
    mCollision = new CollisionComponent;
    mTransform->mMatrix.scale(mTransform->mScale);
    mCollision->setBoundingSphere(factoryPtr->getColli(5)->radius, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->ObjectName = "Enemy_" + to_string(factoryPtr->getCount());
    mNameComp->ObjectID = factoryPtr->getCount();
    factoryPtr->addCount();

}



