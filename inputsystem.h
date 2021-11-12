#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "camera.h"
#include "input.h"
#include "player.h"
#include "inputcomponent.h"
#include "enemy.h"

class InputSystem
{
public:
    InputSystem();

    void update(Player *mPlayer,Camera *mCamera,Input mInput);
    void CheckActiveInput();
    void setCameraSpeed(Camera *mCurrentCamera,float value);

private:
    CollisionSystem* mColSystem;
    ShapeFactory* mFactory;
};

#endif // INPUTSYSTEM_H
