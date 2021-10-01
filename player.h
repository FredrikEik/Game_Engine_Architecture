#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player();
    ~Player();
    virtual void move(float dx, float dy, float dz) override;
private:
    Triangle* trianglePtr;
    float mx, my, mz; // posisjon
};

#endif // PLAYER_H
