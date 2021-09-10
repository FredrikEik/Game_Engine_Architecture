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

    MeshComponent* MeshComp = nullptr;
    TextureComponent* TextureComp = nullptr;

};

#endif // CUBE_H
