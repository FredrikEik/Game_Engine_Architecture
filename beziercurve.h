#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <matrix4x4.h>
#include "visualobject.h"
#include <list>
#include <vector2d.h>

class BezierCurve:public VisualObject
{
public:
    BezierCurve();
    BezierCurve(QVector2D PointA, QVector2D PointB, QVector2D PointC, QVector2D PointD);
    ~BezierCurve();

    QVector3D Bezipoint(QVector2D PointA, QVector2D PointB, QVector2D PointC, QVector2D PointD, double size);

    void init() override;
    void draw() override;
private:
    QVector3D bezi;
};

#endif // BEZIERCURVE_H
