#ifndef SPHERE_H
#define SPHERE_H

#pragma once

#include "gameobject.h"

class Sphere : public GameObject
{
public:
    Sphere();
    ~Sphere();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

private:

};

#endif // SPHERE_H
