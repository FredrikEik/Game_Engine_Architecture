#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "visualobject.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "particlesystem.h"
#include "collisionsystem.h"
#include "shapefactory.h"

/**
*MovementSystem - Inneholder funksjoner for å regne ut movement.
*update -  Sjekker om spiller eller kamera input blir brukt.
*setCameraSpeed - Setter kameras hastighet
*movePlayer - Beveger
*moveEnemy -
*moveForward -
*move -
*centerPlayer -
*moveParticles -
*setPlayerForward -

 */

class MovementSystem
{
public:
    MovementSystem();
    Input mInput;
    void update(Camera *mCamera, Player* dPlayer, Input mInput);
    void setCameraSpeed(Camera *mCamera, float value);
    void moveUnit(VisualObject* vo, MovementComponent *moveC);
    void moveParticles(gsl::Vector3D mColor, Particle* par);
    void moveForward(VisualObject* vo, MovementComponent* m);
    void move(VisualObject* vo, float dx, float dy, float dz);
    void rotateForward(MovementComponent* m);
    void centerPlayer();
private:
    void setPlayerForward(float dx, float dz);

    ParticleSystem  *parSys{nullptr};
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* mFactory{nullptr};
    Player* mPlayer{nullptr};
};

#endif // MOVEMENTSYSTEM_H
