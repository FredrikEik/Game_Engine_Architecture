#ifndef SHAPE_H
#define SHAPE_H

#pragma once

#include "gameobject.h"

class Shape : public GameObject
{
public:
    Shape();
    ~Shape();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

};

#endif // SHAPE_H
