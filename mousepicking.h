#ifndef MOUSEPICKING_H
#define MOUSEPICKING_H

#include "vector3d.h"
#include "vector2d.h"
#include <QVector4D>
#include "camera.h"
#include "input.h"
#include "matrix4x4.h"
#include <math.h>

#define null 0

class MousePicking
{
public:
    MousePicking();

    void MousePicker();
    gsl::Matrix4x4 projectionMatrix;
    gsl::Matrix4x4 viewMatrix;
    void update();
    Camera *mCamera;
    Input *mInput;
    gsl::Vector3D ray_nds;
    QVector4D clipCoords;
    QVector4D ray_eye;


private:
    gsl::Vector3D GetcurrentRay(){return currentRay;};
    gsl::Vector3D currentRay;
    gsl::Vector2D getNormalizedDeviceCoords(float mouseX, float mouseY);
    gsl::Vector3D calculateMouseRay();
    QVector4D toEyeCoords(QVector4D clipCoords);
    gsl::Vector3D toWorldCoords(QVector4D eyeCoords);
    gsl::Vector3D getPointOnRay(gsl::Vector3D ray, float distance);


};

#endif // MOUSEPICKING_H
