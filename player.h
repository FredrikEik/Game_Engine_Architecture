#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"
#include "shapefactory.h"

/**
@class Player - Player som arver fra VisualObject.
*Lager nødvendige new components som spiller trenger i kontruktør.

 */

class Player : public VisualObject
{
public:
    Player(ShapeFactory* f);
    ~Player();
    MovementComponent* mMoveComp{nullptr};


private:
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* factoryPtr{nullptr};

};

#endif // PLAYER_H
