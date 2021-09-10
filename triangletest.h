#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "gameobject.h"
#include "components.h"

class TriangleTest : public GameObject
{
public:
    TriangleTest();
    ~TriangleTest() override;

    void init() override;
    void draw() override;
};

#endif // TRIANGLETEST_H
