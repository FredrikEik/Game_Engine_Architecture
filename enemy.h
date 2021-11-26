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
    float speed = 0.050;
    float PSpeed = 0.005;


    void rotateY();
    gsl::Vector3D mForward{0.f, 0.f, 1.f};
    gsl::Vector3D Toplayer{0,0,0};
    void rotateForwardV();
    void CheckRotation();
    int angle = 0;
    void goToPlayer(gsl::Vector3D pPos);

private:
        CollisionSystem *mColsystem;
        Player *mPlayer;
        ShapeFactory* factoryPtr;
        gsl::Vector3D dir;
};

#endif // ENEMY_H