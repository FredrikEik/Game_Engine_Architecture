#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "camera.h"
#include "player.h"
#include "inputcomponent.h"

class InputSystem
{
public:
    InputSystem();
    Input mInput;
    void CheckActiveInput();
    void update(Camera *mCamera, Player* mPlayer, Input mInput);
    void setCameraSpeed(Camera *mCamera, float value);
};

#endif // INPUTSYSTEM_H
