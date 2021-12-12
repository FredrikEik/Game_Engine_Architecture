#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "gameobject.h"

class Billboard : public GameObject
{
public:
    Billboard();
    ~Billboard();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
};

#endif // BILLBOARD_H
