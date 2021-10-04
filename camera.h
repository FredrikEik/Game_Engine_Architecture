#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "gameobject.h"
#include "components.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */
class Camera : public GameObject
{
public:
    Camera(float fieldOfView, float aspectRatio);

    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();
    void init() override;
    void draw() override;

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsl::Vector3D &position);

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);
    void updateFrustumPos(float fieldOfView, float aspectRatio);

    gsl::Vector3D position() const;
    gsl::Vector3D up() const;

    TransformComponent* getTransformComponent(){return transformComp;}
    MeshComponent* getMeshComponent(){return meshComp;}
    MaterialComponent* getMaterialComponent(){return materialComp;}
    FrustumCollisionComponent* getFrustumComponent(){return frustumComp;}

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

    float farplaneX;
    float farplaneY;
    float farplaneZ;
    float nearplaneX;
    float nearplaneY;
    float nearplaneZ;


    TransformComponent* transformComp;
    MeshComponent* meshComp;
    MaterialComponent* materialComp;
    FrustumCollisionComponent* frustumComp;
};

#endif // CAMERA_H
