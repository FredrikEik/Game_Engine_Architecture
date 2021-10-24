#ifndef PLAYER_H
#define PLAYER_H

#include "vector3d.h"
#include "visualobject.h"
#include "shapefactory.h"
#include "collisionsystem.h"
#include "inputcomponent.h"

class Player : public VisualObject
{
public:
    Player();
    ~Player();
    virtual void move(float dx, float dy, float dz) override;
    InputComponent* mInputComp;


private:
    float mx, my, mz; // posisjon
    CollisionSystem *mColsystem;

};

#endif // PLAYER_H
