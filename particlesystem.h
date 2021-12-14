#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "visualobject.h"

struct ParticleComponent
{
    gsl::Vector3D pos{0,0,0};
    gsl::Vector3D direction{0,0,0};
    gsl::Vector3D velocity{0,0,0};
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
