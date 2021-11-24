#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "lassurface.h"


class LasSurface;
class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;
    void move(float dt) ;
    void setSurface(GameObject* surface) { LasSurface = surface; }
    double radius = 0.25;

    GameObject* LasSurface;

private:
                        //lager vektorer som brukes til forskjellig utregning
    gsl::Vector3D v0,v1,v2;               //vektor for vertices
    gsl::Vector3D barycentricCord;         //vektor for barysentriske kordinater
    gsl::Vector3D normalvektor =0;
    gsl::Vector3D nyPosisjon;
    gsl::Vector3D hastighet{0.0f,0.0f,0.0f};
    gsl::Vector3D akselerasjon{0.0f,0.0f,0.0f};
    gsl::Vector3D gAkselerasjon{0.0f,0.0f,9.80565f};
    gsl::Vector3D gKraft;
    float masseIKG{0.05f};              //massen til ballen


    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};

#endif // ROLLINGBALL_H
