#include "camerainputcomponent.h"

void CameraInputComponent::update(Camera *mCamera, Input mInput)
{
    mCamera->setSpeed(0.f);
    if(mInput.RMB == true)
    {
        if(mInput.W == true)
            mCamera->updateHeigth(mCameraSpeed);
        if(mInput.S == true)
            mCamera->updateHeigth(-mCameraSpeed);
        if(mInput.D == true)
            mCamera->moveRight(mCameraSpeed);
        if(mInput.A == true)
            mCamera->moveRight (-mCameraSpeed);
        if(mInput.Q == true)
            mCamera->setSpeed (-mCameraSpeed);
        if(mInput.E == true)
            mCamera->setSpeed (mCameraSpeed);
    }
}
