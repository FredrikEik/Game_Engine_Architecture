#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "camera.h"
#include "player.h"
#include "movementcomponent.h"
#include "collisionsystem.h"
#include "shapefactory.h"

class MovementSystem
{
public:
    MovementSystem();
    Input mInput;
    void CheckActiveInput();
    void update(Camera *mCamera, Player* mPlayer, Input mInput);
    void setCameraSpeed(Camera *mCamera, float value);
private:
    CollisionSystem* mColSystem;
    ShapeFactory* mFactory;
};

#endif // MOVEMENTSYSTEM_H
