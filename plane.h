#ifndef PLANE_H
#define PLANE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"

class Plane: public VisualObject
{
public:
    Plane();
    ~Plane();

    void init() override;
    void draw() override;
};

#endif // PLANE_H
