#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "camera.h"
#include "inputcomponent.h"

class InputSystem : public InputComponent
{
public:
    InputSystem();
    Camera mCurrentCamera;
    Input mInput;
    void CheckActiveInput();

    void setCameraSpeed(float value);
};

#endif // INPUTSYSTEM_H
