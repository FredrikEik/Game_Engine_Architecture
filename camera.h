#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"
/**
FrustumComponent holder raw data som blir brukt for å kalkulere frustum.
*mFOV - Field of view velger vinkel til hvorstort området vi vil se.
*mAspectratio - Aspectratio velger sideforhold.
*mFarPlaneDistance - FarPlaneDistance velger hvor langt unna Farplane skal være.
*mNearPlaneDistance - NearPlaneDistance velger hvor nærme NearPlane skal være.
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
FrustumSystem - inneholder all nødvendige funksjoner for å regne ut frustum.
*calculateFrustumVectors - Kalkulerer planene til frustumet.
*makeFrustumLines - Konstruerer frustumets linker ved hjelp av gsl::Vector3D plan.
*updateFrustumPos - Oppdaterer posisjonen til frustum når kamera beveger seg (uferdig kode, tar ikke kamera sin posisjon i betraktning).
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
Camera inneholder all nødvendige funksjoner og variabler.
*Pitch - Regner ut rotasjon rundt mRight.
*yaw - Regner ut rotasjon rundt mUp.
*updateRightVector - Oppdaterer mRight.
*updateForwardVector - Oppdaterer mForward.
*update - Oppdaterer posisjon og matrix.
*calculateProjectionMatrix - kalkulerer projeksjons matrise i henhold med FrustumComponents.
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
