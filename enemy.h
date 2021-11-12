#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"
#include "collisionsystem.h"
#include "player.h"

class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy();
    void moveToPlayer(Player *mPlayer, gsl::Vector3D Ppos);
    float speed = 0.3f;

    bool Checkmove = false;

private:
        CollisionSystem *mColsystem;
        Player *mPlayer;
        ShapeFactory* factoryPtr;
        gsl::Vector3D dir;
};

#endif // ENEMY_H
