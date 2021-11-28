#include "camera.h"
#include <QDebug>
#include "cameracomponent.h"
Camera::Camera()
{

    Cam.mViewMatrix.setToIdentity();
    //mViewMatrix.setToIdentity();
    Cam.mProjectionMatrix.setToIdentity();

    //mProjectionMatrix.setToIdentity();
    Cam.mYawMatrix.setToIdentity();
    //mYawMatrix.setToIdentity();
    Cam.mPitchMatrix.setToIdentity();
    //mPitchMatrix.setToIdentity();
}

Camera::Camera(float fovIn, float nearPlaneDistanceIn, float farPlaneDistanceIn)
{

    Cam.mViewMatrix.setToIdentity();
    Cam.mProjectionMatrix.setToIdentity();


    Cam.mYawMatrix.setToIdentity();
    Cam.mPitchMatrix.setToIdentity();

    mFrustum.mFOVvertical = fovIn;
    mFrustum.mNearPlaneDistance = nearPlaneDistanceIn;
    mFrustum.mFarPlaneDistance = farPlaneDistanceIn;

    updateForwardVector();

    calculateFrustumVectors();
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    Cam.mPitch -=degrees;
    //mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    Cam.mYaw -= degrees;
    //mYaw -= degrees;
    updateForwardVector();
}

void Camera::updateRightVector()
{
    Cam.mRight = Cam.mForward ^Cam.mUp;
    Cam.mRight.normalize();
    //mRight = mForward^mUp;
    //mRight.normalize();
//    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{
    Cam.mRight = gsl::Vector3D(1.f, 0.f, 0.f);
    Cam.mRight.rotateY(Cam.mYaw);
    Cam.mRight.normalize();
    Cam.mUp = gsl::Vector3D(0.f, 1.f, 0.f);
    Cam.mUp.rotateX(Cam.mPitch);
    Cam.mUp.rotateY(Cam.mYaw);
    Cam.mUp.normalize();
    Cam.mForward = (Cam.mUp)^(Cam.mRight);

    updateRightVector();
}

void Camera::update()
{
    Cam.mProjectionMatrix.perspective(mFrustum.mFOVvertical,mFrustum.mAspectRatio,mFrustum.mNearPlaneDistance,mFrustum.mFarPlaneDistance*100);
    Cam.mYawMatrix.setToIdentity();
    Cam.mPitchMatrix.setToIdentity();

    Cam.mPitchMatrix.rotateX(Cam.mPitch);
    Cam.mYawMatrix.rotateY(Cam.mYaw);

    Cam.mPosition -= (Cam.mForward)* (Cam.mSpeed);

    Cam.mViewMatrix = Cam.mPitchMatrix* Cam.mYawMatrix;
    Cam.mViewMatrix.translate(-Cam.mPosition);
}

void Camera::setPosition(const gsl::Vector3D &position)
{
   Cam.mPosition = position;
}

void Camera::setSpeed(float speed)
{
    Cam.mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    Cam.mPosition.y += deltaHeigth;
}

void Camera::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
    gsl::Vector3D right = Cam.mRight;
    right.y = 0.f;
    Cam.mPosition += right * delta;
}

gsl::Vector3D Camera::position() const
{
    return Cam.mPosition;
}

gsl::Vector3D Camera::up() const
{
    return Cam.mUp;
}

gsl::Vector3D Camera::forward() const
{
    return Cam.mForward;
}

float Camera::getCameraSpeed()
{
    return Cam.mSpeed;
}

void Camera::calculateFrustumVectors()
{
    float halfVheight = mFrustum.mFarPlaneDistance * tanf(gsl::deg2radf(mFrustum.mFOVvertical/2)); //calculate the lenght of the opposite
    float halfHwidth = halfVheight * mFrustum.mAspectRatio;

    float horisontalHalfAngle = abs(gsl::rad2degf(atan2f(halfHwidth, mFrustum.mFarPlaneDistance)));
    float verticalHalfAngle = abs(gsl::rad2degf(atan2f(halfVheight, mFrustum.mFarPlaneDistance)));

    gsl::Vector3D tempVector;
    //rightplane vector = mRight rotated by FOV around camera up
    tempVector = Cam.mRight;
    tempVector.axisAngleRotation(-horisontalHalfAngle, Cam.mUp);
    mFrustum.mRightPlane = tempVector.normalized();

    //leftPlane vector = mRight rotated by FOV+180 around camera up
    tempVector = Cam.mRight;
    tempVector.axisAngleRotation(horisontalHalfAngle - 180.f, Cam.mUp);
    mFrustum.mLeftPlane = tempVector.normalized();

    //rightplane vector = mRight rotated by FOV around camera up
    tempVector = Cam.mUp;
    tempVector.axisAngleRotation(verticalHalfAngle, Cam.mRight);
    mFrustum.mToptPlane = tempVector.normalized();

    //leftPlane vector = mRight rotated by FOV+180 around camera up
    tempVector = Cam.mUp;
    tempVector.axisAngleRotation(-verticalHalfAngle - 180.f,Cam.mRight);
    mFrustum.mBottomPlane = tempVector.normalized();

}

void Camera::setLookAt(gsl::Vector3D lookAt)
{
    Cam.mPosition = lookAt;
}
