#ifndef MARIOCUBE_H
#define MARIOCUBE_H

#pragma once

#include "gameobject.h"
#include "cube.h"

class MarioCube : public GameObject
{
public:
    MarioCube();
    ~MarioCube();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

    BoxCollisionComponent* getBoxCollisionComponent() {return boxCollisionComp;}
    Cube* badLodMesh;

private:
    BoxCollisionComponent* boxCollisionComp;

};

#endif // MARIOCUBE_H
