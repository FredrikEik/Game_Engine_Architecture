#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player(ShapeFactory* f);
    ~Player();
    void centerPlayer();
    MovementComponent* mMoveComp;
private:
    float mx, my, mz; // posisjon
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;
};

#endif // PLAYER_H
