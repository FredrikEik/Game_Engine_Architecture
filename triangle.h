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

    void init() override;
    void draw() override;

private:
    MeshComponent* meshComp;
    MaterialComponent* materialComp;
};

#endif // TRIANGLE_H
