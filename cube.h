#ifndef CUBE_H
#define CUBE_H


#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"

class Cube : public VisualObject
{
public:
    Cube();
    ~Cube() override;

    void init() override;
    void draw() override;
};


#endif // CUBE_H
