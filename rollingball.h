#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall();
    void init(GLint matrixUniform);
    void draw();
    void move(float dt);
    void setSurface(GameObject* surface) { triangle_surface = surface; }
    void barycentricCoordinates();
protected:
    GameObject* triangle_surface;
private:
    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    //float gravity = -9.8f;
    gsl::Vector3D acceleration = {0, 0, 0};
    gsl::Vector3D velocity = {0, 0, 0};
    gsl::Vector3D gAcceleration = {0, 0, 9.8};
    gsl::Vector3D gForce = {0, 0, 0};
    float massKG = 0.01f;

};
#endif // ROLLINGBALL_H
