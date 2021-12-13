#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "vector3d.h"
#include "gameobject.h"
#include "particle.h"

class ParticleSystem
{
public:
    ParticleSystem();
    static std::vector<Particle*> particles;
    static void update();
    static void addParticle(Particle* particle);
};

#endif // PARTICLESYSTEM_H
