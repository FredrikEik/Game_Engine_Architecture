#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
//#include "Player.h"
#include "inputcomponent.h"

class PlayerInputComponent : public InputComponent
{
public:
//  virtual void update(Player* player);

private:
  Input* mInput;
  static const int mWalkSpeed = 1;
};

#endif // PLAYERINPUTCOMPONENT_H
