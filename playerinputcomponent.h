#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
#include "inputcomponent.h"
#include <player.h>

class PlayerInputComponent : public InputComponent
{
public:
    void update(Player *mPlayer, Input mInput);

};

#endif // PLAYERINPUTCOMPONENT_H
