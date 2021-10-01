#include "mousepicking.h"



MousePicking::MousePicking()
{

}

void MousePicking::MousePicker()
{

//    this->mCamera->mProjectionMatrix = projectionMatrix;
//    this->mCamera->mViewMatrix = viewMatrix;
    //    // heavily influenced by: http://antongerdelan.net/opengl/raycasting.html
    //    // viewport coordinate system
    //    // normalized device coordinates
    //    float x = (2.f * mouseX) / Width - 1.f;
    //    float y = 1.f - (2.f * mouseY) / Height;
    //    float z = 1.f;
    //    gsl::Vector3D rayNormalizedDeviceCoordinates = gsl::Vector3D(x, y, z);

    //    // 4D homogeneous clip coordinates
    //    QVector4D rayClip = QVector4D(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);

    //    // 4D eye (camera) coordinates
    //    QVector4D ray_eye = (mProjectionMatrix.inverse()) * rayClip;
    //    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.f, 0.f);

    //    // 4D world coordinates
    //    gsl::Vector3D ray_wor = (((mViewMatrix.inverse()) * ray_eye).x(),((mViewMatrix.inverse()) * ray_eye).y(),((mViewMatrix.inverse()) * ray_eye).z());
    //    ray_wor.normalize();
}

//void MousePicking::update()
//{
//    viewMatrix = mCamera->mViewMatrix;
//    currentRay = calculateMouseRay();
//}

//gsl::Vector2D MousePicking::getNormalizedDeviceCoords(int mouseX, int mouseY,int width,int height)
//{
//    float x = (2.f*mouseX)/width - 1.0f;
//    float y = 1.0f - (2.f*mouseY)/height;
//    float z = -1.0f;
//    ray_nds = gsl::Vector3D(x,y,z);
//}

//gsl::Vector3D MousePicking::calculateMouseRay()
//{
//    float mouseX = mInput->MOUSEX;
//    float mouseY = mInput->MOUSEY;
//    gsl::Vector2D normalizedCoords = getNormalizedDeviceCoords(mouseX,mouseY);
//    clipCoords = QVector4D(normalizedCoords.x,normalizedCoords.y,-1.f,1.f);
//    QVector4D eyeCoords = toEyeCoords(clipCoords);
//    gsl::Vector3D worldRay = toWorldCoords(eyeCoords);
//    return worldRay;
//}

//QVector4D MousePicking::toEyeCoords(QVector4D clipCoords)
//{

//  ray_eye = (mCamera->mProjectionMatrix.inverse()) * clipCoords;
//  ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.f, 0.f);
//  return QVector4D(ray_eye);
//}

//gsl::Vector3D MousePicking::toWorldCoords(QVector4D eyeCoords)
//{
//    gsl::Vector3D ray_wor = (((mCamera->mViewMatrix.inverse()) * ray_eye).x(),((mCamera->mViewMatrix.inverse()) * ray_eye).y(),((mCamera->mViewMatrix.inverse()) * ray_eye).z());
//    gsl::Vector3D mouseRay = gsl::Vector3D(ray_wor.x, ray_wor.y, ray_wor.z);
//    mouseRay.normalize();
//    return mouseRay;
//}

//gsl::Vector3D MousePicking::getPointOnRay(gsl::Vector3D ray, float distance)
//{
//    gsl::Vector3D camPos = mCamera->position();
//    gsl::Vector3D start = gsl::Vector3D(camPos.x, camPos.y, camPos.z);
//    gsl::Vector3D scaledRay = gsl::Vector3D(ray.x * distance, ray.y * distance, ray.z * distance);
//    return 0;
//}
