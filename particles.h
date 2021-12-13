#ifndef PARTICLES_H
#define PARTICLES_H
#include "visualobject.h"
#include "vector3d.h"

class Particles : public VisualObject
{
public:
    Particles(ShapeFactory* f,Player *mPlayer);
    ~Particles();
    void update(int frameCount);

    gsl::Vector3D getPosition();
    gsl::Vector3D PathDirection{0,0,0};
    gsl::Vector3D direction{0,0,0};
    gsl::Vector3D mPosition{0,0,0};
    gsl::Vector3D mVelocity{0,0,0};
    gsl::Vector3D mColor{0,0,0};
    bool isAlive = true;
    void getVec();
    int secCount = 0;
    void reset(Player *mPlayer);

private:
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;





};



#endif // PARTICLES_H
