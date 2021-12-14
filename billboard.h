#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "gameobject.h"
#include "vector4d.h"

class Billboard : public GameObject
{
public:
    Billboard();
    ~Billboard();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;

    gsl::Vector4D BillboardPos_worldspace;
};

#endif // BILLBOARD_H
