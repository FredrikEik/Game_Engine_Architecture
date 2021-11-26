#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getMesh(5);
    mCollision = new CollisionComponent;
    //mCollision = factoryPtr->getColli(5);
    mTransform->mMatrix.scale(2);
    mCollision->setBoundingSphere(0.4, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->ObjectName = "Enemy";
    mNameComp->ObjectID = factoryPtr->getCount();
    factoryPtr->addCount();

}


Enemy::~Enemy()
{


}
void Enemy::moveEnemy()
{
    move(mForward.x*speed,mForward.y*speed,mForward.z*speed);

}

void Enemy::rotateForwardV()
{
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


void Enemy::goToPlayer(gsl::Vector3D pPos)
{
    gsl::Vector3D ePos {mTransform->mPosition.x,0,mTransform->mPosition.z};
    gsl::Vector3D dir = pPos - ePos;

    qDebug()<<dir;
    if(dir.x>dir.z)
        dir = {dir.x,0,0};
    else if(dir.x<dir.z)
        dir = {0,0,dir.z};
    else
        qDebug()<<"Somethings wrong with move";
    dir.normalize();
    mForward = dir;

}


