#ifndef CUBE_H
#define CUBE_H

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

#endif // CUBE_H
