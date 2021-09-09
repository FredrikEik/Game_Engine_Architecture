#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "gameobject.h"

class Triangle : public GameObject
{
public:
    Triangle();
    ~Triangle() override;

    void init() override;
};

#endif // TRIANGLE_H
