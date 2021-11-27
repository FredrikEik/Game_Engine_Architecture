#include "particles.h"
#include <QElapsedTimer>

Particles::Particles(ShapeFactory* f)
{

    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getMesh(3);
    mCollision = new CollisionComponent;
    mCollision = factoryPtr->getColli(3);
   // mTransform->mMatrix.scale(2);
    //mCollision->setBoundingSphere(0.01, mTransform->mPosition);
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->ObjectName = "Particles";
    mNameComp->ObjectID = factoryPtr->getCount();
    factoryPtr->addCount();

//    mPlayer->mBackward.x = direction.x;
//    mPlayer->mBackward.z = direction.z;
//    direction.x = mVelocity.x;
//    direction.z = mVelocity.z;

}

Particles::~Particles()
{


}

void Particles::update(float dt)
{
    mPosition += mVelocity * dt;
}

void Particles::setPosition(gsl::Vector3D position)
{
    mPosition = position;
}

gsl::Vector3D Particles::getPosition()
{
    return mPosition;
}

void Particles::getRandParticle()
{

}
