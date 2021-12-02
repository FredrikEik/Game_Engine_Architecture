#ifndef BSPLINEPATH_H
#define BSPLINEPATH_H

#include "gameobject.h"
#include "sphere.h"
#include "trianglesurface.h"


class bSplinePath : public GameObject
{
public:
    bSplinePath();
    bSplinePath(GameObject* ball, GameObject* surface);
    ~bSplinePath();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    void makeControlPoints();
    void setOwnerBall(GameObject* ball) { ownerBall = ball; }
    void setSurface(GameObject* triangle_surface){surface = triangle_surface;}

private:

    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    gsl::Vector3D currentPosition;
    int old_index{0};
    gsl::Vector3D velocity{0.0,0.0,0.0};
    gsl::Vector3D acceleration{0.0,0.0,0.0};
    gsl::Vector3D gForce{0.f,-9.80565f,0.f};
    gsl::Vector3D gAcceleration{0.0,9.8,0.0};
    GameObject* ownerBall;
    GameObject* surface;

    float mass = 0.01;

};

#endif // BSPLINEPATH_H
