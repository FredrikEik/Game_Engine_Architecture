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
    void moveEnemy();
    float speed = 0.03f;

    bool checkMove = false;
    void rotateY();
    gsl::Vector3D mForward{0.f, 0.f, -1.f};

private:
        CollisionSystem *mColsystem;
        Player *mPlayer;
        ShapeFactory* factoryPtr;
        gsl::Vector3D dir;
};

#endif // ENEMY_H
