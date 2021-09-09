#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "visualobject.h"

class Triangle : public VisualObject
{
public:
    Triangle();
    ~Triangle() override;

    void init() override;
};

#endif // TRIANGLE_H
/*
Entity Triangle

MeshComponent
TransformComponent


0

meshComponents[0] = new MeshComponent(TRIANGLE_MESH)
transformComponent[0]

*/


