#include "mousepicker.h"


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
    tog::vec4 clipCoords = tog::vec4(normalizedCoords.getX(), normalizedCoords.getY(), -1.f, 1.f);
    tog::vec4 eyeCoords = toEyeCoords(clipCoords);
    gsl::Vector3D worldRay = toWorldCoords(eyeCoords);
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
    gsl::Matrix4x4 invertedProjectionMatrix = projectionMatrix;
    invertedProjectionMatrix.inverse();
    tog::vec4 eyeCoords = tog::vec4(invertedProjectionMatrix, clipCoords);
    return tog::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
}

gsl::Vector3D MousePicker::toWorldCoords(tog::vec4 eyeCoords)
{
    gsl::Matrix4x4 invertedViewMatrix = viewMatrix;
    invertedViewMatrix.inverse();
    tog::vec4 rayWorld = tog::vec4(invertedViewMatrix, eyeCoords);
    gsl::Vector3D mouseRay = gsl::Vector3D(rayWorld.x, rayWorld.y, rayWorld.z);
    mouseRay.normalize();
    return mouseRay;
}
