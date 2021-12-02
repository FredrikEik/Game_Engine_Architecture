#ifndef SPHERE_H
#define SPHERE_H

#pragma once

#include "gameobject.h"
#include "trianglesurface.h"

class Sphere : public GameObject
{
public:
    Sphere();
    ~Sphere();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    void setSurface(GameObject* surface) { triangle_surface = surface; }
    GameObject* getSurface(){return triangle_surface;}


private:

    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsl::Vector3D velocity{0.0,0.0,0.0};
    gsl::Vector3D acceleration{0.0,0.0,0.0};
    gsl::Vector3D gForce{0.0,0.0,0.0};
    gsl::Vector3D gAcceleration{0.0,9.8,0.0};
    GameObject* spherePath;
    GameObject* triangle_surface;
    float mass = 0.01;

};

#endif // SPHERE_H
