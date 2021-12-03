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
    void predictPath(float dt);
    void createBSpline();

    void setPosition(float x, float y, float z);

    GameObject* LASsurface;
    GameObject* mSpline;
    std::vector<Vertex> mPredictedPath;

    bool bPredictPath{false};
    bool bDrawPath{false};

private:

    float gConstant{9.80565f};
    float massInKg {10.f};
    gsl::Vector3D velocity;
    gsl::Vector3D acceleration;
    gsl::Vector3D predictedVelocity;
    gsl::Vector3D predictedAcceleration;
    gsl::Vector3D gForce;
    gsl::Vector3D prevPosition;

    //gsl::Vector3D friction{0.0f, 0.0f, 0.0f};
    //float frictionStrength{2.0f};


    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};

#endif // ROLLINGBALL_H
