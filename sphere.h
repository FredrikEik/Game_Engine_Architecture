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
    void movement();
    InputComponent input;

private:
    float movementSpeed = 0.3f;
    gsl::Vector3D cameraOffset;
    gsl::Vector3D cameraTarget;
    gsl::Vector3D getcameraTarget () {return cameraTarget;}

};

#endif // SPHERE_H
