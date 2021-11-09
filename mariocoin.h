#ifndef MARIOCOIN_H
#define MARIOCOIN_H

#pragma once

#include "gameobject.h"

class MarioCoin : public GameObject
{
public:
    MarioCoin();
    ~MarioCoin();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;


    //BoxCollisionComponent* getBoxCollisionComponent() {return boxCollisionComp;}

private:
    //BoxCollisionComponent* boxCollisionComp;
};

#endif // MARIOCOIN_H
