#include "camera.h"

#include "soundsystem.h"

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

    SoundSystem::getInstance()->updateListener(mPosition, mForward, mUp);

}

void Camera::calculateProjectionMatrix()
{
    mProjectionMatrix.perspective(mFrustum.mFOVvertical, mFrustum.mAspectRatio, mFrustum.mNearPlaneDistance, mFrustum.mFarPlaneDistance);
    calculateFrustumVectors();
    Logger::getInstance()->logText("AspectRatio " + std::to_string(mFrustum.mAspectRatio));
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
    right.y = 0.0f;
    mPosition += right * delta;
}

void Camera::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
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

//    Logger::getInstance()->logText(mName + "-Camera horisontal FOV: " + std::to_string(horisontalHalfAngle) +
//                                   + ", vertical FOV: " + std::to_string(mFrustum.mFOVvertical) +
//                                   ", Right vector: " + mFrustum.mRightPlane.getAsString());
}
