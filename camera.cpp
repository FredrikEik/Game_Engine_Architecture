#include "camera.h"
#include <QDebug>

Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    mYaw -= degrees;
    updateForwardVector();
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

void Camera::calculateProjectionMatrix()
{
    mProjectionMatrix.perspective(mFrustum.mFOV, mFrustum.mAspectRatio, mFrustum.mNearPlaneDistance, mFrustum.mFarPlaneDistance);
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

gsl::Vector3D Camera::up() const
{
    return mUp;
}

gsl::Vector3D Camera::right() const
{
    return mRight;
}


void Camera::calculateFrustumVectors()
{
    //Does not take into account the pitch of the camera!
    //So it will not be accurate when camera is pitching

    gsl::Vector3D tempVector;
    //rightplane vector = mRight rotated by FOV around up
    tempVector = mRight;
    tempVector.rotateY(-mFrustum.mFOV);
    mFrustum.mRightPlane = tempVector;

    //leftPlane vector = mRight rotated by FOV+180 around up
    tempVector = mRight;
    tempVector.rotateY(mFrustum.mFOV + 180.f);
    mFrustum.mLeftPlane = tempVector;

    //nearplane =
    tempVector.setX(tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance);
    tempVector.setY((tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance)/mFrustum.mAspectRatio);
    tempVector.setZ(mFrustum.mFarPlaneDistance);

}
