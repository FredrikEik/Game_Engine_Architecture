#include "player.h"

player::player(GameObject* mesh)
{
    mMesh = mesh;
}

player::~player()
{

}

void player::update(float deltaTime)
{
    if (!bCanJump)
    {
        qDebug() << "velocity: " << mVelocity << " deltaTime: " << deltaTime << " = " << mVelocity / deltaTime;
        mHeight += mVelocity / deltaTime;
        mVelocity -= 9.81f / deltaTime;
        //qDebug() << "velocity: " << mVelocity << " height: " << mHeight;
    }
    if (mHeight <= 0.51f)
    {
        mHeight = 0.51f;
        bCanJump = true;
        mVelocity = 0.f;
    }
    if (mMesh)
    {
        gsl::Vector3D position = mMesh->getTransformComp()->mTrueScaleMatrix.getPosition();

        if (position.x + mDt > 5.f)
        {
            position.x = 5.f;
            mDt = 0.f;
        }
        else if (position.x + mDt < 1.f)
        {
            position.x = 1.f;
            mDt = 0.f;
        }

        mMesh->getTransformComp()->mTrueScaleMatrix.setPosition(position.x + mDt, mHeight, position.z + (-1 /  deltaTime));
        mMesh->getTransformComp()->mMatrix.setPosition(position.x + mDt, mHeight, position.z + (-1 /  deltaTime));
        mDt = 0.f;
    }
}

void player::Move(float dt)
{
    mDt = dt;
}

void player::Jump()
{
    if (bCanJump)
    {
        bCanJump = false;
        mVelocity = 6.f;
    }
}
