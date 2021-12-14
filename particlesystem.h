#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "visualobject.h"
#include "vector3d.h"

/**
*ParticleComponent - Holder nødvendige data for å regne ut particlenes fart, farge og retning.
*Particle - Lager new componenter som partikler trenger slik som navn, matriale osv...
*ParticleSystem - Regner ut nødvendige funksjoner for partikler, bruker paricleComponent sine data.
 */

struct ParticleComponent
{
    gsl::Vector3D pos{0,0,0};
    gsl::Vector3D direction{0,0,0};
    gsl::Vector3D velocity{0,0,0};
    gsl::Vector3D color{1,1,1};
    bool isAlive = true;
    int secCount = 0;
};

class Particle : public VisualObject
{
public:
    Particle(ShapeFactory* f, Player *mPlayer);
    ~Particle();
    ParticleComponent* mPC;
private:
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;
};

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
