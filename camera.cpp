#include "camera.h"
#include "math_constants.h" //gotta include this for frustum calculations.
#include <QDebug>

Camera::Camera(float fovIn, float nearPlaneDistanceIn, float farPlaneDistanceIn)
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mFrustum.mFOVvertical = fovIn;
    mFrustum.mNearPlaneDistance = nearPlaneDistanceIn;
    mFrustum.mFarPlaneDistance = farPlaneDistanceIn;

    updateForwardVector();

    calculateFrustumVectors();
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

void Camera::setPitch(float pitch)
{
    mPitch = pitch;
    updateForwardVector();
}

void Camera::setYaw(float yaw)
{
    mYaw = yaw;
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
    mUp.rotateY(mYaw);
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
    mProjectionMatrix.perspective(mFrustum.mFOVvertical, mFrustum.mAspectRatio, mFrustum.mNearPlaneDistance, mFrustum.mFarPlaneDistance);
    calculateFrustumVectors();
}

void Camera::setPosition(const gsl::Vector3D &position)
{
    mPosition = position;
}

gsl::Vector3D Camera::getPosition()
{
    return mPosition;
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

void Camera::calculateFrustumVectors()
{
    float halfVheight = mFrustum.mFarPlaneDistance * tanf(gsl::deg2radf(mFrustum.mFOVvertical/2)); //calculate the lenght of the opposite
    float halfHwidth = halfVheight * mFrustum.mAspectRatio;

    float horisontalHalfAngle = abs(gsl::rad2degf(atan2f(halfHwidth, mFrustum.mFarPlaneDistance)));

    gsl::Vector3D tempVector;
    //rightplane vector = mRight rotated by FOV around camera up
    tempVector = mRight;
    tempVector.axisAngleRotation(-horisontalHalfAngle, mUp);
    mFrustum.mRightPlane = tempVector.normalized();

    //leftPlane vector = mRight rotated by FOV+180 around camera up
    tempVector = mRight;
    tempVector.axisAngleRotation(horisontalHalfAngle - 180.f, mUp);
    mFrustum.mLeftPlane = tempVector.normalized();

}
