#ifndef CAMERAINPUTCOMPONENT_H
#define CAMERAINPUTCOMPONENT_H
#include "camera.h"
#include "inputcomponent.h"

class CameraInputComponent : public InputComponent
{
public:
    void update(Camera *mCamera, Input mInput);
};

#endif // CAMERAINPUTCOMPONENT_H
