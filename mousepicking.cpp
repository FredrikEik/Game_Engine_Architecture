#include "mousepicking.h"

MousePicking::MousePicking()
{

}

void MousePicking::PickObjectOnScreen(int mouseX, int mouseY)
{

    // heavily influenced by: http://antongerdelan.net/opengl/raycasting.html
    // viewport coordinate system
    // normalized device coordinates
    float x = (2.f * mouseX) / Width - 1.f;
    float y = 1.f - (2.f * mouseY) / Height;
    float z = 1.f;
    gsl::Vector3D rayNormalizedDeviceCoordinates = gsl::Vector3D(x, y, z);

    // 4D homogeneous clip coordinates
    QVector4D rayClip = QVector4D(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);

    // 4D eye (camera) coordinates
    QVector4D ray_eye = (mProjectionMatrix.inverse()) * rayClip;
    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.f, 0.f);



    // 4D world coordinates
    gsl::Vector3D ray_wor = (((mViewMatrix.inverse()) * ray_eye).x(),((mViewMatrix.inverse()) * ray_eye).y(),((mViewMatrix.inverse()) * ray_eye).z());
    ray_wor.normalize();

}
