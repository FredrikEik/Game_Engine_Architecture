#ifndef CUBE_H
#define CUBE_H

#pragma once

#include "gameobject.h"

class Cube : public GameObject
{
public:
    Cube();
    ~Cube();

    void draw() override;
    void init() override;

private:

    MeshComponent* MeshComp;
    TextureComponent* TextureComp;

};

#endif // CUBE_H
