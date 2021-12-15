#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"
#include "shapefactory.h"

/**
Player-klassen arver fra VisualObject og legger på en MovementComponent.
Her sender vi også inn shapeFactory-peker i konstruktøren for å hente ut
riktig mesh og kollisjon.
 */

class Player : public VisualObject
{
public:
    Player(ShapeFactory* f);
    ~Player(){};
    MovementComponent* mMoveComp;
};

#endif // PLAYER_H
