#ifndef CAMERAINPUTCOMPONENT_H
#define CAMERAINPUTCOMPONENT_H
#include "camera.h"
#include "inputcomponent.h"

class CameraInputComponent : public InputComponent
{
public:
  virtual void update() override;
  virtual void update(Camera* mCamera);

private:
  Input* mInput;
  static const int mCameraSpeed = 1;
};

#endif // CAMERAINPUTCOMPONENT_H
