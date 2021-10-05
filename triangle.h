#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "gameobject.h"

class Triangle : public GameObject
{
public:
    Triangle();
    ~Triangle() override;

    void init();
    void draw();

};

#endif // TRIANGLE_H
