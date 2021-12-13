#include "particlesystem.h"

ParticleSystem::ParticleSystem(ShapeFactory* f,Player *mPlayer)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getParticleMesh();
    mCollision = new CollisionComponent;
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->mName = "Particles_" + to_string(factoryPtr->getCount());
    mNameComp->objectID = factoryPtr->getCount();
    factoryPtr->addCount();
    direction = {(-1)*mPlayer->mMoveComp->mForward.x,0,(-1)*mPlayer->mMoveComp->mForward.z};
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays( 1, &mMesh->mVAO );
    glDeleteBuffers( 1, &mMesh->mVBO );
}

void ParticleSystem::update(int frameCount)
{
    if(frameCount == 10 ||frameCount == 20 || frameCount == 30){
        secCount++;}

    if(secCount > 1){
        isAlive = false;
        secCount= 0;
    }
}

void ParticleSystem::getVec()
{
    PathDirection = {direction.x * mVelocity.x,direction.y * mVelocity.y,direction.z * mVelocity.z};
    PathDirection = PathDirection*0.5;
}

void ParticleSystem::reset(Player *mPlayer)
{
    mTransform->mPosition = gsl::Vector3D(mPlayer->mTransform->mPosition.x,mPlayer->mTransform->mPosition.y, mPlayer->mTransform->mPosition.z);
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    direction = {(-1)*mPlayer->mMoveComp->mForward.x,0,(-1)*mPlayer->mMoveComp->mForward.z};
    isAlive = true;
}
