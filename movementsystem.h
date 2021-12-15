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
   I MovementSystem har vi alle funksjoner som har med bevegelse å gjøre.
   Her får vi bruk for blant annet Transform-, Movement-, og Input-components.
   Vi har en vanlig move-funksjon her, i tillegg til mer spesifikke funksjoner
   for objekter i konstant bevegelse, som Player, enemy og partikler.
 */

class MovementSystem
{
public:
    MovementSystem();
    void CheckActiveInput();
    void update(Camera *mCamera, Player* dPlayer, InputComponent mInput);
    void setCameraSpeed(Camera *mCamera, float value);
    void moveUnit(VisualObject* vo, MovementComponent* moveC);
    void moveParticle(gsl::Vector3D color, Particle* par);
    void moveForward(VisualObject* vo, MovementComponent* m);
    void move(VisualObject* vo, float dx, float dy, float dz);
    void rotateForward(MovementComponent* m);
    void centerPlayer();
private:
    void setPlayerForward(float dx, float dz);

    ParticleSystem* parSys{nullptr};
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* mFactory{nullptr};
    Player* mPlayer{nullptr};
};

#endif // MOVEMENTSYSTEM_H
