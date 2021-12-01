#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "planeimport.h"


class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n, GameObject* p);
    ~RollingBall() override;
    virtual void init() override;
    virtual void draw() override;
    void move(float dt);
    //void setSurface(VisualObject* surface) { triangle_surface = surface; }

protected:
    GameObject* plane;
private:
    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsl::Vector3D force;
    gsl::Vector3D acceleration{0.f};
    gsl::Vector3D mVelocity{0.f};
};

#endif // ROLLINGBALL_H
