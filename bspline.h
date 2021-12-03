#ifndef BSPLINE_H
#define BSPLINE_H

#pragma once

#include "gameobject.h"

class BSpline : public GameObject
{
public:
    BSpline(GameObject* rollingBall);
    ~BSpline();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    void predictPath(float step);

    GameObject* surfaceRef;
    GameObject* ballRef;

    std::vector<Vertex> controlPoints;
    gsl::Vector3D currentCPoint;

    bool bPredictPath{true};


private:
    float gConstant{9.80565f};
    float ballMassInKg{10.f};
    gsl::Vector3D velocity;
    gsl::Vector3D acceleration;
};

#endif // BSPLINE_H
