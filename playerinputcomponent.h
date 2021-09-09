#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
#include "player.h"
#include "inputcomponent.h"

class PlayerInputComponent : public InputComponent
{
public:
    void update(Player *mPlayer, Input mInput);
};

#endif // PLAYERINPUTCOMPONENT_H
