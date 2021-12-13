#ifndef PLAYER_H
#define PLAYER_H

#include "movementcomponent.h"
#include "visualobject.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player(ShapeFactory* f);
    ~Player();
    void movePlayer();
    void setVector(float dx, float dz);
    void centerPlayer();
    void CheckPlayerWall(Square* w);
    MovementComponent* mMoveComp{nullptr};
    gsl::Vector3D mForward{0,0,-1};

private:
    float mx, my, mz; // posisjon
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* factoryPtr{nullptr};

};

#endif // PLAYER_H
