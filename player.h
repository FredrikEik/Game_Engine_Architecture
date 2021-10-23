#ifndef PLAYER_H
#define PLAYER_H

#include "inputcomponent.h"
#include "visualobject.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player();
    ~Player();
    virtual void move(float dx, float dy, float dz) override;
    InputComponent* mInputComp;
private:
    float mx, my, mz; // posisjon
    CollisionSystem* mColSystem;
};

#endif // PLAYER_H
