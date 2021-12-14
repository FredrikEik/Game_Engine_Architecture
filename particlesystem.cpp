#include "particlesystem.h"

Particle::Particle(ShapeFactory* f,Player *mPlayer)
{
    factoryPtr = f;
    mPC = new ParticleComponent;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = factoryPtr->getParticleMesh();
    mCollision = new CollisionComponent;
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
    mNameComp->mName = "Particles_" + to_string(factoryPtr->getCount());
    mNameComp->objectID = factoryPtr->getCount();
    factoryPtr->addCount();
    mPC->direction = {(-1)*mPlayer->mMoveComp->mForward.x,0,(-1)*mPlayer->mMoveComp->mForward.z};
}

Particle::~Particle()
{
    glDeleteVertexArrays( 1, &mMesh->mVAO );
    glDeleteBuffers( 1, &mMesh->mVBO );
}

void ParticleSystem::update(int frameCount, ParticleComponent* p)
{
    if(frameCount == 10 ||frameCount == 20 || frameCount == 30){
        p->secCount++;}

    if(p->secCount > 1){
        p->isAlive = false;
        p->secCount= 0;
    }
}

void ParticleSystem::getVec(ParticleComponent* p)
{
    p->pos = {p->direction.x * p->velocity.x, p->direction.y * p->velocity.y, p->direction.z * p->velocity.z};
    p->pos = p->pos*0.5;
}

void ParticleSystem::reset(Player *mPlayer, Particle* mPart, ParticleComponent* p)
{
    mPart->mTransform->mPosition = gsl::Vector3D(mPlayer->mTransform->mPosition.x,mPlayer->mTransform->mPosition.y, mPlayer->mTransform->mPosition.z);
    mPart->mTransform->mMatrix.setPosition(mPart->mTransform->mPosition.x, mPart->mTransform->mPosition.y, mPart->mTransform->mPosition.z);
    p->direction = {(-1)*mPlayer->mMoveComp->mForward.x,0,(-1)*mPlayer->mMoveComp->mForward.z};
    p->isAlive = true;
}
