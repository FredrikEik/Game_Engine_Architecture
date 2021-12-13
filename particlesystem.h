#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "visualobject.h"

class ParticleSystem : public VisualObject
{
public:
    ParticleSystem(ShapeFactory* f, Player *mPlayer);
    ~ParticleSystem();
    void update(int frameCount);

    gsl::Vector3D PathDirection{0,0,0};
    gsl::Vector3D direction{0,0,0};
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

#endif // PARTICLESYSTEM_H
