#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */

struct Frustum
{
    float mFarPlaneDistance{100.f};   //used to make projection matrix
    float mNearPlaneDistance{0.1f};  //used to make projection matrix
    float mFOVvertical{45.f};                //used to make projection matrix
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

    void pitch(float degrees);
    void yaw(float degrees);
    void setPitch(float pitch);
    void setYaw(float yaw);
    void updateRightVector();
    void updateForwardVector();
    void update();
    void calculateProjectionMatrix();

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsl::Vector3D &position);
    gsl::Vector3D getPosition();

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    gsl::Vector3D position() const;
    gsl::Vector3D up() const;

    gsl::Vector3D mForward{0.f, 0.f, -1.f};
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    gsl::Vector3D mPosition{0.f, 0.f, 0.f};

    Frustum mFrustum;

    std::string mName{"none"};

private:
    float mPitch{0.f};
    float mYaw{0.f};

    void calculateFrustumVectors();

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
};

#endif // CAMERA_H
