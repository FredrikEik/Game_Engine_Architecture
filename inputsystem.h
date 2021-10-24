#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "camera.h"
#include "input.h"
#include "player.h"
#include "inputcomponent.h"

class InputSystem
{
public:
    InputSystem();


    void updateP(Player *mPlayer,Input mInput);
    void updateC(Camera *mCurrentCamera,Input mInput);
    void CheckActiveInput();
    void setCameraSpeed(Camera *mCurrentCamera,float value);






};

#endif // INPUTSYSTEM_H
