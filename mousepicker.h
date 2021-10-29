#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H

#include "gsl/matrix4x4.h"
#include "gsl/vector2d.h"
#include "camera.h"
#include "MathStuff/MathStuff.h"


class MousePicker
{
public:
    MousePicker(Camera *cam);

    void update(int x, int y, int width, int height);

    gsl::Vector3D getCurrentRay() { return currentRay; }

private:
    gsl::Vector3D currentRay;
    gsl::Matrix4x4 projectionMatrix;
    gsl::Matrix4x4 viewMatrix;
    Camera *camera;

    gsl::Vector3D calculateMouseRay(int mouse_x, int mouse_y, int width, int height);
    gsl::Vector2D getNormalizedDeviceCoords(int mouse_x, int mouse_y, int width, int height);
    tog::vec4 toEyeCoords(tog::vec4 clipCoords);
    gsl::Vector3D toWorldCoords(tog::vec4 eyeCoords);

    bool TestRayOBBIntersection(gsl::Vector3D rayOrigin, gsl::Vector3D rayDirection, gsl::Vector3D aabb_min, gsl::Vector3D aabb_max, gsl::Vector3D objectPosition , float& intersection_distance);


};

#endif // MOUSEPICKER_H
