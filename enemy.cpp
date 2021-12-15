#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMoveComp = new MovementComponent;
    mTransform->mScale = 0.2;
    mMoveComp->mSpeed = 0.025;
    mMesh = f->getMesh(5);
    mCollision = new CollisionComponent;
    mTransform->mMatrix.scale(mTransform->mScale);
    mCollision->setBoundingSphere(f->getColli(5)->radius, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->mName = "Enemy_" + to_string(f->getCount());
    mNameComp->objectID = f->getCount();
    f->addCount();
}
