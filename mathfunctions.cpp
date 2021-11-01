#include "mathfunctions.h"
#include <cmath>
#include <qvector2d.h>
#include <qvector3d.h>

gsm::MathFunctions::MathFunctions()
{

}

float gsm::MathFunctions::degreesToRadians(float angle)
{
    return angle * (M_PI / 180.0f);
}

void gsm::MathFunctions::rotateX(float angle, QVector3D &OutVector)
{
    angle = degreesToRadians(angle);

    QVector3D direction{};

    direction.setX(OutVector.x());
    direction.setY((OutVector.y() * std::cos(angle)) + (OutVector.z() * (-std::sin(angle))));
    direction.setZ((OutVector.y() * std::sin(angle)) + (OutVector.z() * std::cos(angle)));

    OutVector = direction;
}

void gsm::MathFunctions::rotateY(float angle, QVector3D &OutVector)
{
    angle = degreesToRadians(angle);

    QVector3D direction{};

    direction.setX((OutVector.x() * std::cos(angle)) + (OutVector.z() * std::sin(angle)));
    direction.setY(OutVector.y());
    direction.setZ((OutVector.x() * (-std::sin(angle))) + (OutVector.z() * std::cos(angle)));

    OutVector = direction;
}

void gsm::MathFunctions::rotateX(float angle, QMatrix4x4 &OutMatrix)
{
    angle = degreesToRadians(angle);

    QMatrix4x4 temp
    (
        1.f,   0.f,       0.f,    0.f,
        0.f, std::cos(angle),  std::sin(angle), 0.f,
        0.f, -std::sin(angle), std::cos(angle), 0.f,
        0.f,   0.f,       0.f,    1.f
    );

    OutMatrix = OutMatrix * temp;
}

void gsm::MathFunctions::rotateY(float angle, QMatrix4x4 &OutMatrix)
{
    angle = degreesToRadians(angle);

    QMatrix4x4 temp
    (
        std::cos(angle), 0.f, -std::sin(angle), 0.f,
        0.f,       1.f,      0.f,       0.f,
        std::sin(angle), 0.f,  std::cos(angle), 0.f,
        0.f,       0.f,      0.f,       1.f
    );

    OutMatrix = OutMatrix * temp;
}

QVector3D gsm::MathFunctions::barycentricCoordinates(const QVector2D &pos, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3)
{
    QVector2D p12 = p2 - p1;
    QVector2D p13 = p3-p1;
    QVector3D n = QVector3D::crossProduct(p12.toVector3D(), p13.toVector3D());
    float areal_123 = n.length(); // dobbelt areal
    QVector3D baryc; // til retur. Husk// u
    QVector2D p = p2 - pos;
    QVector2D q = p3 - pos;
    n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
    baryc.setX(n.z()/areal_123);// v
    p = p3 - pos;
    q = p1 - pos;

    n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
    baryc.setY(n.z()/areal_123);// w
    p = p1 - pos;
    q = p2 - pos;
    n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
    baryc.setZ(n.z()/areal_123); // u
    return baryc;
}
