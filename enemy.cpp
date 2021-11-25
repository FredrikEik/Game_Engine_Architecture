#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getMesh(5);
    mCollision = factoryPtr->getColli(5);
    mTransform->mMatrix.scale(0.2);
    mCollision->setBoundingSphere(0.25, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->mName = "Enemy";
    mNameComp->objectID = factoryPtr->getCount();
    factoryPtr->addCount();
}

void Enemy::moveEnemy()
{
    move(mForward.x*speed,mForward.y*speed,mForward.z*speed);
}
