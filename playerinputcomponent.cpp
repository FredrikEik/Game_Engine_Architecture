#include "playerinputcomponent.h"

void PlayerInputComponent::update(Player *mPlayer, Input mInput)
{
    if(mInput.RMB == false)
    {
//        mColSystem->CheckCollision(mPlayer->mCollision, mFactory->myShapes[1]->mCollision);
        if(mInput.W == true)
//            if(mColSystem->resultX)
            mPlayer->move(0,0,-mPlayerSpeed);
        if(mInput.S == true)
            mPlayer->move(0,0, mPlayerSpeed);
        if(mInput.D == true)
            mPlayer->move(mPlayerSpeed,0,0);
        if(mInput.A == true)
            mPlayer->move(-mPlayerSpeed,0,0);
    }
}
