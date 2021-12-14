#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"
/**
@struct FrustumComponent holder raw data som blir brukt for å kalkulere frustum.
@variable mFOV - Field of view velger vinkel til hvorstort området vi vil se.
@variable mAspectratio - Aspectratio velger sideforhold.
@variable mFarPlaneDistance - FarPlaneDistance velger hvor langt unna Farplane skal være.
@variable mNearPlaneDistance - NearPlaneDistance velger hvor nærme NearPlane skal være..
 */

struct FrustumComponent
{
    float mFOV{90.f};
    float mAspectRatio{1.f};
    float mFarPlaneDistance{100.f};
    float mNearPlaneDistance{0.1};
};
class Camera;

/**
@class FrustumSystem - inneholder all nødvendige funksjoner for å regne ut frustum.
@fn calculateFrustumVectors - Kalkulerer planene til frustumet.
@fn makeFrustumLines - Konstruerer frustumets linker ved hjelp av gsl::Vector3D plan.
@fn updateFrustumPos - Oppdaterer posisjonen til frustum når kamera beveger seg (uferdig kode, tar ikke kamera sin posisjon i betraktning).
 */

class FrustumSystem : public VisualObject
{
public:
    FrustumSystem(Camera* C);
    ~FrustumSystem();
    void init() override;
    void calculateFrustumVectors();
    void makeFrustumLines();
    void updateFrustumPos();
    bool insideFrustum(gsl::Vector3D pos);

private:
    Camera* mCam{nullptr};
    FrustumComponent* mFrustum;
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

/**
@class Camera inneholder all nødvendige funksjoner og variabler.
@fn Pitch - Regner ut rotasjon rundt mRight.
@fn yaw - Regner ut rotasjon rundt mUp.
@fn updateRightVector - Oppdaterer mRight.
@fn updateForwardVector - Oppdaterer mForward.
@fn update - Oppdaterer posisjon og matrix.
@fn calculateProjectionMatrix - kalkulerer projeksjons matrise i henhold med FrustumComponents.
 */


class Camera
{
public:
    Camera();
    ~Camera();
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

    MovementComponent* mMoveComp{nullptr};
    gsl::Vector3D position() const;
    gsl::Vector3D up() const;
    gsl::Vector3D right() const;

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
