#ifndef BSPLINE_H
#define BSPLINE_H

#pragma once

#include "gameobject.h"

class BSpline : public GameObject
{
public:
    BSpline(std::vector<Vertex> cPoints);
    ~BSpline();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

private:

    std::vector<Vertex> controlPoints;

};

#endif // BSPLINE_H
