#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"

class Cube:public VisualObject
{
public:
    Cube();
    ~Cube();

    void draw() override;
    void init() override;
};

#endif // CUBE_H
