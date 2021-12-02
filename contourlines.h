#ifndef CONTOURLINES_H
#define CONTOURLINES_H

#include "gameobject.h"


class contourLines : public GameObject
{
public:
    contourLines();
    ~contourLines();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
};

#endif // CONTOURLINES_H
