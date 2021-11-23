#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H
#include "octahedronball.h"

class LASsurface;
class Rollingball : public OctahedronBall
{
public:
    Rollingball(int n);
    ~Rollingball();
    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;
    void move(float dt);

    void setPosition(float x, float y, float z);

    GameObject* LASsurface;

private:

    gsl::Vector3D velocity{0.0f, 0.0f, 0.0f};
    gsl::Vector3D acceleration{0.0f, 0.0f, 0.0f};
    gsl::Vector3D gForce;
    gsl::Vector3D gAcceleration{0.0f, 0.0f, -9.80565f};
    gsl::Vector3D friction{0.0f, 0.0f, 0.0f};
    float frictionStrength{2.0f};
    float massInKg {0.05f};


    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};

#endif // ROLLINGBALL_H
