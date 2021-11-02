#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "cameracomponent.h"

/**
  Holds normals to the different planes of the frustum
  and some other necessary data
 */ // Taken from Ole.Experiment branch on GEA2021
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
    Camera();
    Camera(float fovIn = 45, float nearPlaneDistanceIn = 0.1f, float farPlaneDistanceIn = 500.f);

    cameraComponent Cam;
    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();

    void setPosition(const gsl::Vector3D &position);

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    gsl::Vector3D position() const;
    gsl::Vector3D up() const;
    gsl::Vector3D forward() const;

    //frustum
    Frustum mFrustum;
private:
    //frostum
    void calculateFrustumVectors();
};

#endif // CAMERA_H
