#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "components.h"

/**
  Holds normals to the different planes of the frustum
  and some other necessary data
 */
struct Frustum
{
    float mFarPlaneDistance{0.f};   //used to make projection matrix
    float mNearPlaneDistance{0.f};  //used to make projection matrix
    float mFOVvertical{0.f};                //used to make projection matrix
    float mAspectRatio{1.7777778f};        //used to make projection matrix - set to 16/9 as default

    //Normals for the side planes - for frustum culling
    //Make sure these are stored normalized!
    gsl::Vector3D mRightPlane;
    gsl::Vector3D mLeftPlane;
    gsl::Vector3D mToptPlane;
    gsl::Vector3D mBottomPlane;

    //NearPlane and FarPlane normal is the same as Camera::mForward - just moved (and flipped 180 for NearPlane)
};


class Camera
{
public:
    Camera(float fovIn = 45, float nearPlaneDistanceIn = 0.1f, float farPlaneDistanceIn = 500.f);

    ///Tilts camera pitch. Positive number tilts upwards
    void pitch(float degrees);
    ///Rotates camera. Positive number rotates with clock
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();
    void calculateProjectionMatrix();

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsl::Vector3D &position);

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    void setCameraSpeed(float value);

    gsl::Vector3D mForward{0.f, 0.f, -1.f};
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    gsl::Vector3D mPosition{0.f, 0.f, 0.f};

    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};

    Frustum mFrustum;

    float mPitch{0.f};
    float mYaw{0.f};
private:
    void calculateFrustumVectors();

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector

    SoundListenerComponent mListener;
};

#endif // CAMERA_H
