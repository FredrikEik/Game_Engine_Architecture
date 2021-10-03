#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "cameracomponent.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */
class Camera
{
public:
    Camera();

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
private:

};

#endif // CAMERA_H
