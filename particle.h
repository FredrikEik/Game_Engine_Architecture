#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"
#include "camera.h"

/**
    \brief Class for creating a particle emitter.
    \author Lars Joar Bjørkeland with help from Johannes Skjeltorp-Borgaas
    \date 15/12/2021
 */

class particle : public GameObject
{
public:
    particle();
    ~particle();

    void draw() override;
    ///Makes verteces and sets up indices. Sets up and binds particle buffers
    void init() override;
    void move(float x, float y, float z) override;
    ///Updates the particles based on given behavior
    void update(float deltaTime, Camera* camera);
    ///Creates particles from Partile Blueprint
    void spawnParticles(float deltaTime, gsl::Vector3D emitterPosition, gsl::Vector3D cameraPosition);

private:

    ParticleComponent* emitter = new ParticleComponent;



};

#endif // PARTICLE_H
