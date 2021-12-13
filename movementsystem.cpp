#include "movementsystem.h"

MovementSystem:: MovementSystem()
{

}

void MovementSystem::update(Player *mPlayer,Camera *mCamera, Input mInput)
{
    MovementComponent* i;
       mCamera->setSpeed(0.f);
       if(mInput.RMB == true)
       {
           i = mCamera->mMoveComp;
           if(mInput.W == true)
               mCamera->setSpeed(-i->mCameraSpeed);
           if(mInput.S == true)
               mCamera->setSpeed(i->mCameraSpeed);
           if(mInput.D == true)
               mCamera->moveRight(i->mCameraSpeed);
           if(mInput.A == true)
               mCamera->moveRight (-i->mCameraSpeed);
           if(mInput.Q == true)
               mCamera->updateHeigth (-i->mCameraSpeed);
           if(mInput.E == true)
               mCamera->updateHeigth (i->mCameraSpeed);
       }
       else if(mInput.RMB == false)
       {
           i = mPlayer->mMoveComp;
           if(mInput.W == true)
               //if(mColSystem->playerOnBwallY == false)
                   mPlayer->setVector(0, -1);
           if(mInput.S == true)
               //if(mColSystem->playerOnFwallY == false)
                   mPlayer->setVector(0, 1);
           if(mInput.D == true)
               //if(mColSystem->playerOnRwallX == false)
                   mPlayer->setVector(1, 0);
           if(mInput.A == true)
               //if(mColSystem->playerOnLwallX == false)
                   mPlayer->setVector(-1, 0);
       }
}



void MovementSystem::setCameraSpeed(Camera *mCamera,float value)
{
    MovementComponent* i= mCamera->mMoveComp;
    i->mCameraSpeed += value;

    //Keep within some min and max values
    if(i->mCameraSpeed < 0.01f)
        i->mCameraSpeed = 0.01f;
    if (i->mCameraSpeed > 0.3f)
        i->mCameraSpeed = 0.3f;

}

void MovementSystem::movePlayer(Player *mPlayer)
{
   mPlayer->mMoveComp->PposX={static_cast<int>(mPlayer->mTransform->mPosition.x)};
    mPlayer->mMoveComp->PposZ={static_cast<int>(mPlayer->mTransform->mPosition.z)};

    if(mPlayer->mForward.x > 0)
        mPlayer->mMoveComp->PposX = std::ceil(mPlayer->mTransform->mPosition.x);
    else if(mPlayer->mForward.x <0)
        mPlayer->mMoveComp->PposX = std::floor(mPlayer->mTransform->mPosition.x);
    else{
        if(mPlayer->mForward.z >0)
            mPlayer->mMoveComp->PposZ = std::ceil(mPlayer->mTransform->mPosition.z);
        else if(mPlayer->mForward.z <0)
           mPlayer->mMoveComp-> PposZ = std::floor(mPlayer->mTransform->mPosition.z);
        else
            qDebug() << "error in MovementSystem::movePlayer";}


}

void MovementSystem::moveParticles(gsl::Vector3D mColor, std::vector<Particles*> mParticles, Player *mPlayer)
{
    for(int i{0}; i<static_cast<int>(mParticles.size()); i++){

        mParticles[i]->mVelocity = mColor;
        mParticles[i]->getVec();
        if(mParticles[i]->isAlive == true){
        mParticles[i]->move(mParticles[i]->PathDirection.x,mParticles[i]->PathDirection.y,mParticles[i]->PathDirection.z);}
        else{
            mParticles[i]->reset(mPlayer);
        }

    }
}

void MovementSystem::moveEnemy(int randNum, std::vector<Enemy *> mEnemies)
{


    for(int i{0}; i<static_cast<int>(mEnemies.size()); i++){
        mEnemies[i]->mMoveComp->EposX = {static_cast<int>(mEnemies[i]->mTransform->mPosition.x)};
        mEnemies[i]->mMoveComp->EposZ = {static_cast<int>(mEnemies[i]->mTransform->mPosition.z)};

        if(mEnemies[i]->mForward.x > 0)
            mEnemies[i]->mMoveComp->EposX = std::ceil( mEnemies[i]->mTransform->mPosition.x);
        else if(mEnemies[i]->mForward.x <0)
            mEnemies[i]->mMoveComp->EposX = std::floor(mEnemies[i]->mTransform->mPosition.x);
        else{
            if(mEnemies[i]->mForward.z >0)
                mEnemies[i]->mMoveComp->EposZ = std::ceil(mEnemies[i]->mTransform->mPosition.z);
            else if(mEnemies[i]->mForward.z <0)
                mEnemies[i]->mMoveComp->EposZ = std::floor(mEnemies[i]->mTransform->mPosition.z);
            else
                qDebug() << "error in MovementSystem::moveEnemy";}


    }
}
