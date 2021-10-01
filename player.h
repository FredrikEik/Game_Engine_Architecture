#ifndef PLAYER_H
#define PLAYER_H

#include "vector3d.h"
#include "visualobject.h"
#include "collisionsystem.h"
#include "vertex.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player(std::string filename);



private:
    float mx, my,mz;
    CollisionSystem *mCollision;
    VisualObject *mShape;
};

#endif // PLAYER_H
