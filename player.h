#ifndef PLAYER_H
#define PLAYER_H

#include "vector3d.h"
#include "visualobject.h"

class Player : public VisualObject
{
public:
    Player();
    void move(float x, float y, float z);

private:
    float mx, my,mz;


};

#endif // PLAYER_H
