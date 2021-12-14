#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "visualobject.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "particlesystem.h"
#include "collisionsystem.h"
#include "shapefactory.h"

class MovementSystem
{
public:
    MovementSystem();
    Input mInput;
    void CheckActiveInput();
    void update(Camera *mCamera, Player* dPlayer, Input mInput);
    void setCameraSpeed(Camera *mCamera, float value);
    void movePlayer();
    void moveEnemy(int randNum, std::vector<Enemy*> mEnemies);
    void moveParticle(gsl::Vector3D color, Particle* par);
    void moveForward(VisualObject* vo, MovementComponent* m);
    void move(VisualObject* vo, float dx, float dy, float dz);
    void rotateForward(MovementComponent* m);
private:
    void setPlayerForward(float dx, float dz);

    ParticleSystem* parSys{nullptr};
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* mFactory;
    Player* mPlayer;
};

#endif // MOVEMENTSYSTEM_H
