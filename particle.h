#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"
#include "camera.h"

class particle : public GameObject
{
public:
    particle();
    ~particle();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    void update(float deltaTime, Camera* camera);
    void spawnParticles(float deltaTime, gsl::Vector3D emitterPosition, gsl::Vector3D cameraPosition);
    ParticleComponent* getParticleComp(){return particleComp;}

private:

    ParticleComponent* particleComp;
    ParticleComponent* emitter = new ParticleComponent;
    int maxParticles = 100000;



};

#endif // PARTICLE_H
