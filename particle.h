#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"
#include "camera.h"

class particle : public GameObject
{
public:
    particle();
    particle(Camera* camera);
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
    Camera* cameraRef;



};

#endif // PARTICLE_H
