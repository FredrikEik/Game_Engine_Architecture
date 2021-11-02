#include "mousepicker.h"
#include "cmath"


MousePicker::MousePicker(Camera *cam)
{
    camera = cam;
    projectionMatrix = cam->mProjectionMatrix;
    viewMatrix = cam->mViewMatrix;
}

void MousePicker::update(int x, int y, int width, int height)
{
    viewMatrix = camera->mViewMatrix;
    currentRay = calculateMouseRay(x, y, width, height);
}

gsl::Vector3D MousePicker::calculateMouseRay(int mouse_x, int mouse_y, int width, int height)
{
    gsl::Vector2D normalizedCoords = getNormalizedDeviceCoords(mouse_x, mouse_y, width, height);
    //qDebug() << "normalizedCoords x: " << normalizedCoords.x << " y: " << normalizedCoords.y;
    tog::vec4 clipCoords = tog::vec4(normalizedCoords.getX(), normalizedCoords.getY(), -1.f, 1.f);
    //qDebug() << "clipCoords x: " << clipCoords.x << " y: " << clipCoords.y << " z: " << clipCoords.z << " w: " << clipCoords.w;
    tog::vec4 eyeCoords = toEyeCoords(clipCoords);
    //qDebug() << "eyeCoords x: " << eyeCoords.x << " y: " << eyeCoords.y << " z: " << eyeCoords.z << " w: " << eyeCoords.w;
    gsl::Vector3D worldRay = toWorldCoords(eyeCoords);
    //worldRay.setX(eyeCoords.x);
    //worldRay.setY(eyeCoords.y);
    //worldRay.setZ(eyeCoords.z);
    return worldRay;
}

gsl::Vector2D MousePicker::getNormalizedDeviceCoords(int mouse_x, int mouse_y, int width, int height)
{
    float x = (2.f * mouse_x) / width - 1.f;
    float y = 1.f - (2.f * mouse_y) / height;
    return gsl::Vector2D(x, y);
}

tog::vec4 MousePicker::toEyeCoords(tog::vec4 clipCoords)
{
    projectionMatrix = camera->mProjectionMatrix;
    gsl::Matrix4x4 invertedProjectionMatrix = projectionMatrix;
    invertedProjectionMatrix.inverse();
    qDebug() << "projectionMatrix: copy";
    qDebug() << projectionMatrix.getFloat(0) << projectionMatrix.getFloat(1) << projectionMatrix.getFloat(2) << projectionMatrix.getFloat(3);
    qDebug() << projectionMatrix.getFloat(4) << projectionMatrix.getFloat(5) << projectionMatrix.getFloat(6) << projectionMatrix.getFloat(7);
    qDebug() << projectionMatrix.getFloat(8) << projectionMatrix.getFloat(9) << projectionMatrix.getFloat(10) << projectionMatrix.getFloat(11);
    qDebug() << projectionMatrix.getFloat(12) << projectionMatrix.getFloat(13) << projectionMatrix.getFloat(14) << projectionMatrix.getFloat(15);
    tog::vec4 eyeCoords = tog::Transform(invertedProjectionMatrix, clipCoords);
    return tog::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
}

gsl::Vector3D MousePicker::toWorldCoords(tog::vec4 eyeCoords)
{
    gsl::Matrix4x4 invertedViewMatrix = viewMatrix;
    invertedViewMatrix.inverse();
    tog::vec4 rayWorld = tog::Transform(invertedViewMatrix, eyeCoords);
    //qDebug() << "viewMatrix:";
    //qDebug() << viewMatrix.getFloat(0) << viewMatrix.getFloat(1) << viewMatrix.getFloat(2) << viewMatrix.getFloat(3);
    //qDebug() << viewMatrix.getFloat(4) << viewMatrix.getFloat(5) << viewMatrix.getFloat(6) << viewMatrix.getFloat(7);
    //qDebug() << viewMatrix.getFloat(8) << viewMatrix.getFloat(9) << viewMatrix.getFloat(10) << viewMatrix.getFloat(11);
    //qDebug() << viewMatrix.getFloat(12) << viewMatrix.getFloat(13) << viewMatrix.getFloat(14) << viewMatrix.getFloat(15);
    gsl::Vector3D mouseRay = gsl::Vector3D(rayWorld.x, rayWorld.y, rayWorld.z);
    mouseRay.normalize();
    return mouseRay;
}

bool MousePicker::TestRayHitSphere(gsl::Vector3D sphereCenter, float sphereRadius)
{
    // klarte ikke finne noe forståelig om collision i 3d så stjal litt kode fra ole
    gsl::Vector3D rayOrigin = camera->getPosition();

    gsl::Vector3D rayToObject = sphereCenter - rayOrigin;

    gsl::Vector3D planeNormal = currentRay ^ rayToObject;

    gsl::Vector3D rayNormal = planeNormal ^ currentRay;
    rayNormal.normalize();

    float length = rayToObject * rayNormal;
    length = abs(length);

    /* funker bare for 2d
    gsl::Vector3D t = tog::dotProduct(sphereCenter - rayOrigin, currentRay);
    gsl::Vector3D tToSphereCenter = sphereCenter - t;
    float length = tToSphereCenter.length();
    float x = std::sqrt((sphereRadius * sphereRadius) - (length * length));*/

    if (length <= sphereRadius)
        return true;

    return false;
}

/*
bool MousePicker::TestRayOBBIntersection(gsl::Vector3D rayOrigin, gsl::Vector3D rayDirection, gsl::Vector3D aabb_min, gsl::Vector3D aabb_max, gsl::Vector3D objectPosition , float& intersection_distance)
{
    float tMin = 0.f;
    float tMax = 100000.f;

    gsl::Vector3D OBBposition_worldspace = objectPosition;

    gsl::Vector3D delta = OBBposition_worldspace - rayOrigin;

    gsl::Vector3D xaxis;
    float e = tog::dotProduct(xaxis, delta);
    float f = tog::dotProduct(rayDirection, xaxis);

    float t1 = (e + aabb_min.getX()) / f; // Intersection with the "left" plane
    float t2 = (e + aabb_max.getX()) / f;

    if (t1 > t2) {
        float w = t1;
        t1=t2;
        t2=w;
    }

    if ( t2 < tMax ) tMax = t2;
    if ( t1 > tMin ) tMin = t1;

    if (tMax < tMin )
        return false;
    else
        return true;
}
*/
