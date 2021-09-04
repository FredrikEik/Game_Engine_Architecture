#ifndef CAMERAINPUTCOMPONENT_H
#define CAMERAINPUTCOMPONENT_H
#include "inputcomponent.h"
#include "camera.h"

class CameraInputComponent : public InputComponent
{
public:

    void update(Camera *mCamera);
private:
    Input *mInput;
    static const int CAMERA_SPEED = 1;

};
#endif // CAMERAINPUTCOMPONENT_H
