#include "enemy.h"

Enemy::Enemy(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getMesh(5);
    mCollision = factoryPtr->getColli(5);
    mTransform->mMatrix.scale(2);
    mCollision->setBoundingSphere(0.25, mTransform->mPosition);
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
//    if(angle == 0)
//    {

//    }


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

void Enemy::CheckRotation()
{
    if(mForward.x==1)
        angle = 270;
    if(mForward.x == -1)
        angle = 90;
    if(mForward.z == -1)
        angle = 180;
    if(mForward.z == 1)
        angle =0;



}
