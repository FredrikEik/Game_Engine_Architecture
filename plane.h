#ifndef PLANE_H
#define PLANE_H

#pragma once

#include "gameobject.h"

class Plane : public GameObject
{
public:
    Plane();
    ~Plane();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
};

#endif // PLANE_H
