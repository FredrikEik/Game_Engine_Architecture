#ifndef CUBE_H
#define CUBE_H

#pragma once

#include "gameobject.h"

class Cube : public GameObject
{
public:
    Cube();
    ~Cube();

    void draw();
    void init();

};

#endif // CUBE_H
