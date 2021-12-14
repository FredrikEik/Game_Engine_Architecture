#include "movementsystem.h"

MovementSystem:: MovementSystem()
{

}

void MovementSystem::update(Camera *mCamera, Player* dPlayer, Input mInput)
{
    mPlayer = dPlayer;
    MovementComponent* i;
    mCamera->setSpeed(0.f);
    if(mInput.RMB == true)
    {
        i = mCamera->mMoveComp;
        if(mInput.W == true)
            mCamera->setSpeed(-i->mSpeed);
        if(mInput.S == true)
            mCamera->setSpeed(i->mSpeed);
        if(mInput.D == true)
            mCamera->moveRight(i->mSpeed);
        if(mInput.A == true)
            mCamera->moveRight (-i->mSpeed);
        if(mInput.Q == true)
            mCamera->updateHeigth (-i->mSpeed);
        if(mInput.E == true)
            mCamera->updateHeigth (i->mSpeed);
    }
    else if(mInput.RMB == false)
    {
        if(mInput.W == true)
            setPlayerForward(0, -1);
        if(mInput.S == true)
            setPlayerForward(0, 1);
        if(mInput.D == true)
            setPlayerForward(1, 0);
        if(mInput.A == true)
            setPlayerForward(-1, 0);
    }
}

void MovementSystem::move(VisualObject* vo, float dx, float dy, float dz)
{
    vo->mTransform->mPosition += gsl::Vector3D(dx,dy,dz);
    vo->mTransform->mMatrix.setPosition(vo->mTransform->mPosition.x, vo->mTransform->mPosition.y, vo->mTransform->mPosition.z);

    if(vo->mCollision != nullptr)
    {
        mColSystem->moveBoundingBox(dx, dy, dz, vo->mCollision);
        mColSystem->moveBoundingSphere(dx, dy, dz, vo->mCollision);
    }

}
void MovementSystem::rotateForward(MovementComponent* m)
{
    gsl::Vector3D temp{0,0,0};
    if(m->mForward.x == -1)
        temp = gsl::Vector3D(0,0,-1);
    else if(m->mForward.x == 1)
        temp = gsl::Vector3D(0,0,1);
    else if(m->mForward.z == -1)
        temp = gsl::Vector3D(1,0,0);
    else if(m->mForward.z == 1)
        temp = gsl::Vector3D(-1,0,0);
    else
        qDebug() << "Error rotating Enemy Forward vector";

    m->mForward = temp;
}

void MovementSystem::setCameraSpeed(Camera *mCamera,float value)
{
    MovementComponent* m = mCamera->mMoveComp;
    m->mSpeed += value;

    //Keep within some min and max values
    if(m->mSpeed < 0.01f)
        m->mSpeed = 0.01f;
    if (m->mSpeed > 0.3f)
        m->mSpeed = 0.3f;

}


void MovementSystem::moveParticles(gsl::Vector3D color, Particle* par)
{
    par->mPC->velocity = color;
    parSys->getVec(par->mPC);
    if(par->mPC->isAlive == true){
        move(par, par->mPC->pos.x, par->mPC->pos.y, par->mPC->pos.z);}
    else{
        parSys->reset(mPlayer, par, par->mPC);
    }
}


void MovementSystem::moveUnit(VisualObject* vo, MovementComponent *moveC)
{
    moveC->posX = {static_cast<int>(vo->mTransform->mPosition.x)};
    moveC->posZ = {static_cast<int>(vo->mTransform->mPosition.z)};

    if(moveC->mForward.x > 0)
        moveC->posX = std::ceil( vo->mTransform->mPosition.x);
    else if(moveC->mForward.x <0)
        moveC->posX = std::floor(vo->mTransform->mPosition.x);
    else{
        if(moveC->mForward.z >0)
            moveC->posZ = std::ceil(vo->mTransform->mPosition.z);
        else if(moveC->mForward.z <0)
            moveC->posZ = std::floor(vo->mTransform->mPosition.z);
        else
            qDebug() << "error in MovementSystem::moveEnemy";}
}


void MovementSystem::setPlayerForward(float dx, float dz)
{
    mPlayer->mMoveComp->mForward = {dx, 0, dz};
    mPlayer->mTransform->mMatrix.setRotationToVector(mPlayer->mMoveComp->mForward);
    mPlayer->mTransform->mMatrix.scale(0.5);
}

void MovementSystem::moveForward(VisualObject* vo, MovementComponent* m)
{
    vo->mTransform->mMatrix.setRotationToVector(m->mForward);
    vo->mTransform->mMatrix.scale(vo->mTransform->mScale);
    move(vo, m->mForward.x * m->mSpeed, m->mForward.y * m->mSpeed, m->mForward.z * m->mSpeed);
}

void MovementSystem::centerPlayer()
{
    gsl::Vector3D mPos = mPlayer->mTransform->mPosition;
    gsl::Vector3D mRound {std::round(mPlayer->mTransform->mPosition.x), 0, std::round(mPlayer->mTransform->mPosition.z)};

    float xDiff = mRound.x - mPos.x;
    float zDiff = mRound.z - mPos.z;

    gsl::Vector3D temp(xDiff,0,zDiff);


    mPlayer->mTransform->mPosition += temp;
    mPlayer->mTransform->mMatrix.setPosition(mPlayer->mTransform->mPosition.x, mPlayer->mTransform->mPosition.y, mPlayer->mTransform->mPosition.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mPlayer->mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mPlayer->mCollision);
}