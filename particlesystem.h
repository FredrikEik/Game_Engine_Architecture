#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "visualobject.h"

/**
   I ParticleComponent har vi alt som trengs av rå data til partikler.
   Den trenger retning, fart og en måte å bestemme levetid på.
 */

struct ParticleComponent
{
    gsl::Vector3D pos{0,0,0};
    gsl::Vector3D direction{0,0,0};
    gsl::Vector3D velocity{0,0,0};
    bool isAlive = true;
    int secCount = 0;
};

/**
   Her har vi Particle-klassen som arver fra VisualObject og holder en
   ParticleComponent.
   I konstruktøren henter vi MeshComponent fra shapefactory, som returnerer
   samme mesh, men med forskjellige farger.
 */

class Particle : public VisualObject
{
public:
    Particle(ShapeFactory* f, Player *mPlayer);
    ~Particle();
    ParticleComponent* mPC;
};

/**
   I ParticleSystem gjør vi beregninger av posisjon og levetid for partiklene.
   Vi tar derfor alltid inn en PartikkelKompinent i parametrene.
 */

class ParticleSystem
{
public:
    ParticleSystem(){};
    ~ParticleSystem(){};
    void update(int frameCount, ParticleComponent* p);
    void getVec(ParticleComponent* p);
    void reset(Player *mPlayer, Particle* mPart, ParticleComponent* p);
};

#endif // PARTICLESYSTEM_H
