#ifndef BIGPLANE_H
#define BIGPLANE_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include "vector"
#include "gameobject.h"


class BigPlane : public GameObject
{
public:
    BigPlane();
    ~BigPlane();
    void init() override;
    void draw() override;
};

#endif // BIGPLANE_H
