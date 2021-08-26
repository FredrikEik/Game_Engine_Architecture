#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"

class Triangle : public VisualObject
{
public:
    Triangle();
    ~Triangle() override;

    void init() override;
    void draw() override;
};

#endif // TRIANGLE_H
