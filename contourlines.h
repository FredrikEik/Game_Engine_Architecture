#ifndef CONTOURLINES_H
#define CONTOURLINES_H

#pragma once

#include "gameobject.h"

class ContourLines : public GameObject
{
public:
    ContourLines(std::vector<Vertex> contourPoints);
    ~ContourLines();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

private:

};

#endif // CONTOURLINES_H
