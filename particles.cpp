#include "particles.h"
#include <QElapsedTimer>

Particles::Particles(ShapeFactory* f,Player *mPlayer)
{

    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getParticleMesh();
    mCollision = new CollisionComponent;
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->ObjectName = "Particles";
    mNameComp->ObjectID = factoryPtr->getCount();
    factoryPtr->addCount();
    direction = {(-1)*mPlayer->mForward.x,0,(-1)*mPlayer->mForward.z};

}

Particles::~Particles()
{
    glDeleteVertexArrays( 1, &mMesh->mVAO );
    glDeleteBuffers( 1, &mMesh->mVBO );
}

void Particles::update(int frameCount)
{
    if(frameCount == 10 ||frameCount == 20 || frameCount == 30){
        secCount++;}

    if(secCount > 1){
        isAlive = false;
        secCount= 0;
    }
}

gsl::Vector3D Particles::getPosition()
{
    return mPosition;
}

void Particles::getVec()
{
    PathDirection = {direction.x * mVelocity.x,direction.y * mVelocity.y,direction.z * mVelocity.z};
    PathDirection = PathDirection*0.5;
}

void Particles::reset(Player *mPlayer)
{
    mTransform->mPosition = gsl::Vector3D(mPlayer->mTransform->mPosition.x,mPlayer->mTransform->mPosition.y, mPlayer->mTransform->mPosition.z);
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    direction = {(-1)*mPlayer->mForward.x,0,(-1)*mPlayer->mForward.z};
    isAlive = true;
}



