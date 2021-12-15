#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "gltypes.h"
#include <cmath>
#include <iostream>
#include <QDebug>
#include <QVector3D>

namespace gsl
{

class Vector3D
{
public:
    //Constructors
    Vector3D(GLfloat x_in = 0.f, GLfloat y_in = 0.f, GLfloat z_in = 0.f);
    Vector3D(const int v);
    Vector3D(const double v);

    //Copy constructor
    Vector3D(Vector3D const&vIN);

    //Operators
    const Vector3D& operator=(const Vector3D &rhs);     // v = v
    Vector3D operator+(const Vector3D &rhs) const;      // v + v
    Vector3D operator-(const Vector3D &rhs) const;      // v - v
    Vector3D& operator+=(const Vector3D &rhs);          // v += v
    Vector3D& operator-=(const Vector3D &rhs);          // v -= v
    Vector3D operator-() const;                     // -v
    Vector3D operator*(GLfloat rhs) const;          // v * f
    //Vector3D operator*(Vector3D rhs) const;       //GLfloat operatoren funker ikke med denne???
    Vector3D operator/(GLfloat rhs) const;
    Vector3D operator^(const Vector3D& rhs) const; // v x v  - cross product
    GLfloat operator*(const Vector3D& rhs) const;  // v * v  - dot product
    GLfloat& operator [](int Index);

    //Functions
    GLfloat length() const;
    GLfloat lengthNoSqrt() const;
    void normalize();
    Vector3D normalized();
    static Vector3D cross(const Vector3D &v1, const Vector3D &v2);
    static GLfloat dot(const Vector3D &v1, const Vector3D &v2);

//    GLfloat angleX;
//    GLfloat angleY;
//    GLfloat angleZ;
    void rotateX(GLfloat angleX);
    void rotateY(GLfloat angleY);
    void rotateZ(GLfloat angleZ);

//    GLfloat getRotX() const;
//    GLfloat getRotY() const;
//    GLfloat getRotZ() const;
//    Vector3D RandomNumber(Vector3D *v1, Vector3D *v2);
    //Getters and setters
    GLfloat getX() const;
    void setX(const GLfloat &value);

    GLfloat getY() const;
    void setY(const GLfloat &value);

    GLfloat getZ() const;
    void setZ(const GLfloat &value);

    GLfloat *xP();
    GLfloat *yP();
    GLfloat *zP();


    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const Vector3D &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
        return output;
    }

    //OEF: Adding these again to be able to debug easier. cout does not print until program is finished!
    friend QDebug operator<<(QDebug out, const Vector3D &rhs)
    {
        out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
        return  out;
    }        //    QVector3D getQVector() const;   //for use with qDebug()

    QVector3D getQVector() const
    {
        return QVector3D(x, y, z);
    }   //for use with qDebug()

    GLfloat x;
    GLfloat y;
    GLfloat z;
};

} //namespace

#endif // VECTOR3D_H
