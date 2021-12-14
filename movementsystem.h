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
@class MovementSystem - Inneholder funksjoner for å regne ut movement.
@fn update -  Sjekker om spiller eller kamera input blir brukt.
@param mCamera - sender inn camera peker
@param dPlayer - sender inn player peker
@param mInput - sender inn input.
@fn setCameraSpeed - Setter kameras hastighet.
@param mCamera - Sender inn camera peker
@param value - sender inn en int value så vi kan sende inn tall til f.eks. wheelEvent for å få fortere eller saktere cameraspeed.
@fn moveUnit - Funksjonen blir brukt til både spiller og enemy. Vi bruker den for å sjekke om units er ved vegger i Level,
@param vo - sender inn visualObject peker.
@param moveC - sender inn MovementComponent peker.
@fn moveForward - Både spiller og enemy bruker denne for å gå ihenhold med speed og forward vector
@param vo - sender inn visualObject peker.
@param moveC - sender inn MovementComponent peker.
@fn move - setter posisjon til objektet når dem går, og flytter boundingbox/sphere.
@param vo - sender inn VisualObject peker.
@param dx - sender inn x.
@param dy - sender inn y.
@param dz - sender inn z.
@fn centerPlayer - Sentrerer spiller så spilleren alltid er i midten av tilen
@fn moveParticles - Beveger partiklene.
@param mColor - sender inn color.
@param par - sender inn particle peker.
@fn setPlayerForward - Setter forward vector og roterer matrix.
@param dx - sender inn x.
@param dz - sender inn z.
@fn rotateForward - sier hva mForward blir når den roterer.
@param m - sender inn MovementComponent.
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
