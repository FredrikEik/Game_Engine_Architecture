#include "playerinputcomponent.h"
#include "input.h"

PlayerInputComponent::PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
    switch (Controller::getJoystickDirection())
    {
      case mInput->A==true:
        bjorn.velocity -= WALK_ACCELERATION;
        break;

      case mInput->D==true:
        bjorn.velocity += WALK_ACCELERATION;
        break;
    }
  }
}
