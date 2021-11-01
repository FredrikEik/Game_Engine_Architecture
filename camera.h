#ifndef CAMERA_H
#define CAMERA_H

struct Frustum
{
    //Used to make the projection matrix
    float mFarPlaneDistance{100.0f};
    float mNearPlaneDistance{0.1f};
    float mFOVvertical{45.0f};
    float mAspectRatio{16/9};

    //Normals for the side planes - for frustum culling
    //Make sure these are stored normalized
    gsl::Vector3D mRightPlane;
    gsl::Vector3D mLeftPlane;
    gsl::Vector3D mTopPlane;
    gsl::Vector3D mBottomPlane;
};

class Camera
{
public:
    Camera(float fovIn = 45, float nearPlaneDistanceIn = 0.1f, float farPlaneDistanceIn = 500.f);

    void pitch(float degrees);
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

    std::string mName{"none"};

private:
    void calculateFrustumVectors();

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector

    SoundListenerComponent mListener;
};

#endif // CAMERA_H
