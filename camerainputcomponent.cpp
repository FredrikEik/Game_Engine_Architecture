#include "camerainputcomponent.h"

void CameraInputComponent::update(Camera *mCamera)
{
    if(mInput->RMB)
    {
        if(mInput->W)
            mCamera->setSpeed(CAMERA_SPEED);
        if(mInput->S)
            mCamera->setSpeed(-CAMERA_SPEED);
        if(mInput->D)
            mCamera->moveRight(CAMERA_SPEED);
        if(mInput->A)
            mCamera->moveRight (-CAMERA_SPEED);
        if(mInput->Q)
            mCamera->updateHeigth (-CAMERA_SPEED);
        if(mInput->E)
            mCamera->updateHeigth (CAMERA_SPEED);
    }
}
