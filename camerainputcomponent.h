#ifndef CAMERAINPUTCOMPONENT_H
#define CAMERAINPUTCOMPONENT_H
#include "inputcomponent.h"
#include "camera.h"

class CameraInputComponent : public InputComponent
{
public:
    void update(Camera *mCamera,Input mInput);


};
#endif // CAMERAINPUTCOMPONENT_H
