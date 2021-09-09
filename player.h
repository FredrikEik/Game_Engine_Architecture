#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"

class Player : public VisualObject
{
public:
    Player();
    ~Player();
    void move(float x, float y, float z) ;
private:
    float mx, my, mz; // posisjon
};

#endif // PLAYER_H
