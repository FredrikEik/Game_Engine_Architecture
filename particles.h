#ifndef PARTICLES_H
#define PARTICLES_H
#include "visualobject.h"
#include "vector3d.h"

class Particles : public VisualObject
{
public:
    Particles(ShapeFactory* f);
    ~Particles();
    void update(float dt);

    void setPosition(gsl::Vector3D position);

    gsl::Vector3D getPosition();
    gsl::Vector3D direction;
    void getRandParticle();

private:
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;
    Player *mPlayer;
    gsl::Vector3D mPosition;
    gsl::Vector3D mVelocity;
    gsl::Vector3D mColor;



};



#endif // PARTICLES_H
