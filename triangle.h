#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
<<<<<<< Updated upstream
#include "visualobject.h"
=======
#include "gameobject.h"
>>>>>>> Stashed changes

class Triangle : public VisualObject
{
public:
    Triangle();
    ~Triangle() override;

    void init() override;
    void draw() override;
};

#endif // TRIANGLE_H
