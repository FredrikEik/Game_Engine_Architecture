#include "inputsystem.h"

InputSystem:: InputSystem()
{

}

void InputSystem::updateP(Player *mPlayer, Input mInput)
{
     InputComponent* i= mPlayer->mInputComp;
    if(mInput.RMB == false)
    {
        if(mInput.W == true)
            mPlayer->move(0, 0, -(i->mPlayerSpeed));
        if(mInput.S == true)
           mPlayer->move(0, 0, i->mPlayerSpeed);
        if(mInput.D == true)
            mPlayer->move(i->mPlayerSpeed, 0, 0);
        if(mInput.A == true)
            mPlayer->move(-(i->mPlayerSpeed), 0, 0);
    }
}

void InputSystem::updateC(Camera *mCamera, Input mInput)
{
    InputComponent* i= mCamera->mInputComp;
    mCamera->setSpeed(0.f);
    if(mInput.RMB == true)
    {
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
}



void InputSystem::setCameraSpeed(Camera *mCamera,float value)
{
     InputComponent* i= mCamera->mInputComp;
    i->mCameraSpeed += value;

    //Keep within some min and max values
    if(i->mCameraSpeed < 0.01f)
        i->mCameraSpeed = 0.01f;
    if (i->mCameraSpeed > 0.3f)
        i->mCameraSpeed = 0.3f;

}
