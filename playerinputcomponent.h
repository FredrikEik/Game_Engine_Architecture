#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
#include "inputcomponent.h"

class PlayerInputComponent : public InputComponent
{
public:
    PlayerInputComponent();
    virtual void update(/*sende inn peker til spiller*/);
    Input *mInput;

private:
  static const int WALK_ACCELERATION = 1;
};

#endif // PLAYERINPUTCOMPONENT_H
