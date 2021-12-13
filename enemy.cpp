#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getMesh(5);
    mCollision = new CollisionComponent;
    //mCollision = factoryPtr->getColli(5);
    mTransform->mMatrix.scale(0.2);
    mCollision->setBoundingSphere(factoryPtr->getColli(5)->radius, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->mName = "Enemy_" + to_string(factoryPtr->getCount());
    mNameComp->objectID = factoryPtr->getCount();
    factoryPtr->addCount();
}

void Enemy::moveEnemy()
{
    mTransform->mMatrix.setRotationToVector(mForward);
    mTransform->mMatrix.scale(0.2);
    move(mForward.x*speed,mForward.y*speed,mForward.z*speed);
}

void Enemy::rotateForwardV()
{
    //mTransform->mMatrix.rotateY(90);
    gsl::Vector3D temp{0,0,0};
    if(mForward.x == -1)
        temp = gsl::Vector3D(0,0,-1);
    else if(mForward.x == 1)
        temp = gsl::Vector3D(0,0,1);
    else if(mForward.z == -1)
        temp = gsl::Vector3D(1,0,0);
    else if(mForward.z == 1)
        temp = gsl::Vector3D(-1,0,0);
    else
        qDebug() << "Error rotating Enemy Forward vector";

    mForward = temp;
}
