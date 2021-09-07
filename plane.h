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

private:

    MeshComponent* meshComp;
    MaterialComponent* materialComp;

};

#endif // PLANE_H
