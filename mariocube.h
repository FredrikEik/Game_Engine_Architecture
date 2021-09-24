#ifndef MARIOCUBE_H
#define MARIOCUBE_H

#pragma once

#include "gameobject.h"

class MarioCube : public GameObject
{
public:
    MarioCube();
    ~MarioCube();

    void draw() override;
    void init() override;

};

#endif // MARIOCUBE_H
