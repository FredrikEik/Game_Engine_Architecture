#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <qvector3d.h>
#include <QMatrix4x4>

namespace gsm
{

class MathFunctions
{
public:
    MathFunctions();

    static float degreesToRadians(float angle);
    static void rotateX(float angle, QVector3D &OutVector);
    static void rotateX(float angle, QMatrix4x4 &OutMatrix);
    static void rotateY(float angle, QVector3D &OutVector);
    static void rotateY(float angle, QMatrix4x4 &OutMatrix);

    static QVector3D barycentricCoordinates(const QVector2D &position, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3);
};

}
#endif // MATHFUNCTIONS_H
