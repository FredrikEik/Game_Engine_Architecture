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
    void move(float x, float y, float z) override;

private:

};

#endif // CUBE_H
