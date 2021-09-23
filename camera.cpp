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
    updateRightVector();
    updateForwardVector();
}

void Camera::updateRightVector()
{

    mRight = gsl::Vector3D{1.f, 0.f, 0.f};      //gsl::RIGHT
    mRight.rotateY(mYaw);
    mRight.normalize();

//    mRight = mForward^mUp;
//    mRight.normalize();
//    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{

    mForward = gsl::Vector3D{0.f, 0.f, -1.f};       //gsl::FORWARD;
    mForward.rotateX(mPitch);
    mForward.rotateY(mYaw);     //Rotate around world up
    mForward.normalize();

//    mUp = mRight^mForward;
//    mRight = gsl::Vector3D(1.f, 0.f, 0.f);
//    mRight.rotateY(mYaw);
//    mRight.normalize();
//    mUp = gsl::Vector3D(0.f, 1.f, 0.f);
//    mUp.rotateX(mPitch);
//    mUp.normalize();
//    mForward = mUp^mRight;

//    updateRightVector();
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

    createFrustum(mProjectionMatrix, mViewMatrix);
}

void Camera::setPosition(const gsl::Vector3D &position)
{
    mPosition = position;
}
void Camera::setRotation(const gsl::Vector3D &rotation)
{
    mYaw = rotation.y/2;
    mPitch = -rotation.z;
    updateRightVector();
    updateForwardVector();
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

void Camera::createFrustum(gsl::Matrix4x4 pMatrix, gsl::Matrix4x4 vMatrix)
{
    gsl::Matrix4x4 clPlanes = pMatrix * vMatrix;

    //R frustum plane
    mFrustum[0] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).x,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).x
    };
    mFrustum[0].normalize();

    //L frustum plane
    mFrustum[1] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).x,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).x
    };
    mFrustum[1].normalize();

    //Bt Frustum plane
    mFrustum[2] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).y,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).y
    };
    mFrustum[2].normalize();

    //Tp Frustum plane
    mFrustum[3] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).y,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).y
    };
    mFrustum[3].normalize();

    //B Frustum plane
    mFrustum[4] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).z,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).z
    };
    mFrustum[4].normalize();

    //F Frustum plane
    mFrustum[5] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).z,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).z
    };
    mFrustum[5].normalize();
}
bool Camera::frustumCull(gsl::Vector3D pt)
{
    for (int i{0}; i < 6; ++i)
    {
        if (mFrustum[i].x * pt.x + mFrustum[i].y * pt.y + mFrustum[i].z + pt.z + mFrustum[i].w <= 0)
            return false;
    }

    return true;
}

bool Camera::frustumCull(collider OBB, gsl::Vector3D scale)
{
    bool bInFrustum{false};
    gsl::Vector3D v = scale * OBB.mMinCenter;
    bInFrustum = frustumCull(v);

    return bInFrustum;
}

