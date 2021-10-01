#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
#include "player.h"
#include "inputcomponent.h"
#include "shapefactory.h"

class PlayerInputComponent : public InputComponent
{
public:
    void update(Player *mPlayer, Input mInput);
    ShapeFactory* mFactory;
    CollisionSystem* mColSystem;
};

#endif // PLAYERINPUTCOMPONENT_H
