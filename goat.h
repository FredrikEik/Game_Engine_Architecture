#ifndef GOAT_H
#define GOAT_H

#pragma once

#include "gameobject.h"

class Goat : public GameObject
{
public:
    Goat();
    ~Goat(){};

    void draw() override;
    void init() override;
};

#endif // GOAT_H
