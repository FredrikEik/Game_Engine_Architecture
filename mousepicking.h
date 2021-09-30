#ifndef MOUSEPICKING_H
#define MOUSEPICKING_H

#include "vector3d.h"
#include "vector2d.h"
#include <QVector4D>
#include "camera.h"
#include "inputcomponent.h"
#include "matrix4x4.h"
#include <math.h>


class MousePicking
{
public:
    MousePicking();

    void MousePicker();
    gsl::Matrix4x4 projectionMatrix;
    gsl::Matrix4x4 viewMatrix;
    void update();
    Camera *mCamera;
    Inputco


private:
    gsl::Vector3D GetcurrentRay(){return currentRay;};
    gsl::Vector3D currentRay;
    gsl::Vector2D getNormalizedDeviceCoords(float mouseX, float mouseY);
    gsl::Vector3D calculateMouseRay();


};

#endif // MOUSEPICKING_H
