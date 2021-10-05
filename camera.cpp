#include "camera.h"
#include <QDebug>

Camera::Camera(float fovIn, float nearPlaneDistanceIn, float farPlaneDistanceIn)
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mFrustum.mFOV = fovIn;
    mFrustum.mNearPlaneDistance = nearPlaneDistanceIn;
    mFrustum.mFarPlaneDistance = farPlaneDistanceIn;

    updateForwardVector();

    calculateFrustumVectors();
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    if(mPitch>=-90 && mPitch<=90)   //checks if the camera is turning upside down and stops it
        mPitch -= degrees;

    else if (mPitch<-90)
        mPitch = -90;
    else if (mPitch>90)
        mPitch = 90;

    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    mYaw -= degrees;
    updateForwardVector();
    calculateFrustumVectors();
}

void Camera::updateRightVector()
{
    mRight = mForward^mUp;
    mRight.normalize();
//    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{
    mRight = gsl::Vector3D(1.f, 0.f, 0.f);
    mRight.rotateY(mYaw);
    mRight.normalize();
    mUp = gsl::Vector3D(0.f, 1.f, 0.f);
    mUp.rotateX(mPitch);
    mUp.normalize();
    mForward = mUp^mRight;

    updateRightVector();
}

void Camera::update()
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mPitchMatrix.rotateX(mPitch);
    mYawMatrix.rotateY(mYaw);

    mPosition -= mForward * mSpeed;

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(-mPosition);
}

void Camera::setPosition(const gsl::Vector3D &position)
{
    mPosition = position;
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    mPosition.y += deltaHeigth;
}

void Camera::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
    gsl::Vector3D right = mRight;
    right.y = 0.f;
    mPosition += right * delta;
}

gsl::Vector3D Camera::position() const
{
    return mPosition;
}

//gsl::Vector3D Camera::up() const
//{
//    return mUp;
//}

void Camera::calculateFrustumVectors()
{
    gsl::Vector3D tempVec;
    tempVec = mRight;
    tempVec.rotateY(-mFrustum.mFOV);
    mFrustum.mLeftPlane = tempVec;

    tempVec = mRight;
    tempVec.rotateY(mFrustum.mFOV);
    mFrustum.mLeftPlane = tempVec;
}
