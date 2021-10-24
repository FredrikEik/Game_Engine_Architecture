#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"
#include "inputcomponent.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */

struct FrustumComponent
{
    float mFOV{45.f};
    float mAspectRatio{1.f};
    float mFarPlaneDistance{100.f};
    float mNearPlaneDistance{0.1};
};

class Camera;

class FrustumSystem : public VisualObject
{
public:
    FrustumSystem();
    ~FrustumSystem();
    void init() override;
    void calculateFrustumVectors();
    void makeFrustumLines();
    //void updateFrustumPos(Camera* mCameraPtr);
    FrustumComponent mFrustum;


private:
    gsl::Vector3D mNearPlane;
    gsl::Vector3D mFarPlane;

    gsl::Vector3D mRightTopNear;
    gsl::Vector3D mRightTopFar;
    gsl::Vector3D mRightBotNear;
    gsl::Vector3D mRightBotFar;
    gsl::Vector3D mLeftTopNear;
    gsl::Vector3D mLeftTopFar;
    gsl::Vector3D mLeftBotNear;
    gsl::Vector3D mLeftBotFar;

};

class Camera
{
public:
    Camera();

    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();
    void calculateProjectionMatrix();
    FrustumComponent mFrustumComp;

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsl::Vector3D &position);
    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    gsl::Vector3D position() const;
    gsl::Vector3D up() const;
    gsl::Vector3D right() const;
    InputComponent* mInputComp;

private:
    gsl::Vector3D mForward{0.f, 0.f, -1.f};
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    gsl::Vector3D mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
};

#endif // CAMERA_H
