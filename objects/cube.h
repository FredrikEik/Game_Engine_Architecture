#ifndef CUBE_H
#define CUBE_H


#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"
#include "systems/ecs/Components.h"

class Cube : public VisualObject
{
public:
    Cube();
    ~Cube() override;

    void move(float x, float y, float z);
    void init() override;
    //void draw() override;

   // TransformComponent mTransform;


};


#endif // CUBE_H
