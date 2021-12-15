#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"

/**
  FrustumComponent holder data vi trenger til Frustumet vårt.
  Dette er også verdiene vi har som parameter i Projection-matrix.
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
  FrustumSystem inneholder funksjoner for å sette opp frustum.
  @param Camera* C - Sender inn kameraet frustumet skal festes til.
  calculateFrustumVectors - regner ut NearPlane, FarPlane og punktene vi trenger.
  makeFrustumLines - tegner Frustumet
  insideFrustum - sjekker om objekter er innenfor frustum(uferdig)
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
    //bool insideFrustum(gsl::Vector3D pos);

private:
    Camera* mCam;
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
   Camera-klassen har matriser, komponenter, vectorer og funksjoner.
   Vi har ikke satt opp Camera-klassen i et fullstendig ECS-system ettersom
   vi ikke syntes det var nødvendig.
   Den tar likevel inn komponenter som MovementComponent og FrustumComponent.
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

    MovementComponent* mMoveComp;
    gsl::Vector3D position() const;
    gsl::Vector3D up() const;
    gsl::Vector3D right() const;

private:
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    gsl::Vector3D mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};
    float mSpeed{0.f}; //camera will move by this speed along the mForward vector

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;
};

#endif // CAMERA_H
