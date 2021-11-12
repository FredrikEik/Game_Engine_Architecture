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
    mNameComp->ObjectName = "Enemy";
    mNameComp->ObjectID = factoryPtr->getCount();
    factoryPtr->addCount();

}


Enemy::~Enemy()
{


}
void Enemy::moveToPlayer(Player *mPlayer, gsl::Vector3D Ppos)
{
    //    if(Checkmove == false)
    //    {
    Ppos = mPlayer->mTransform->mPosition;
    gsl::Vector3D Epos = mTransform->mPosition;

    float xDistance = Ppos.x-Epos.x;
    float yDistance = Ppos.y-Epos.y;

    float hypo = sqrt((xDistance * xDistance) + (yDistance * yDistance));


    dir = Ppos - Epos;
    dir.normalize();

    dir = dir*speed;
    qDebug() << hypo;

    if(Checkmove == false)
    {
        move(dir.x,dir.y,dir.z);
    }

}
