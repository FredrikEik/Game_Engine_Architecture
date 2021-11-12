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
    Vector3D();
    Vector3D(GLfloat x_in, GLfloat y_in, GLfloat z_in);
    Vector3D(const int v);
    Vector3D(const float v);
    Vector3D(const double v);

    //Copy constructor
    Vector3D(Vector3D const&vIN);

    //Operators
    const Vector3D& operator=(const Vector3D &rhs);     // v = v
    Vector3D operator+(const Vector3D &rhs) const;      // v + v
    Vector3D operator-(const Vector3D &rhs) const;      // v - v
    Vector3D& operator+=(const Vector3D &rhs);          // v += v
    Vector3D& operator-=(const Vector3D &rhs);          // v -= v
    Vector3D operator-() const;                         // -v
    Vector3D operator*(GLfloat rhs) const;              // v * f
    //Vector3D operator*(Vector3D rhs) const;           //GLfloat operatoren funker ikke med denne???
    Vector3D operator/(GLfloat rhs) const;
    Vector3D operator^(const Vector3D& rhs) const;      // v x v  - cross product
    GLfloat operator*(const Vector3D& rhs) const;       // v * v

    //Functions

    //Returns length of vector in float
    GLfloat length() const;
    GLfloat lengthNoSqrt() const;
    //normalize this vector
    void normalize();
    //returns a normalized version of this vector, without normalizing this vector itself
    Vector3D normalized();
    static Vector3D cross(const Vector3D &v1, const Vector3D &v2);
    static GLfloat dot(const Vector3D &v1, const Vector3D &v2);

    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);

    void axisAngleRotation(float angle, Vector3D axis);

    //Getters and setters
    GLfloat getX() const;
    void setX(const GLfloat &value);

    GLfloat getY() const;
    void setY(const GLfloat &value);

    GLfloat getZ() const;
    void setZ(const GLfloat &value);

    std::string getAsString() const;

    GLfloat *xP();
    GLfloat *yP();
    GLfloat *zP();

    void setAlltoSame(float valueIn);



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

    //private:  //Making these public to avoid having to do things like setY(getY()+3) etc.
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Vector3D barycentricCoordinates(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3)
    {
        Vector3D p12 = p2-p1;
        Vector3D p13 = p3-p1;
        // Ikke bruk z-koordinatene. Vi jobber med trekanter i 2D her og projiserer
        p12.z = 0.0;
        p13.z = 0.0;
        Vector3D n = p12^p13;
        float areal_123 = n.length(); // dobbelt areal

        Vector3D baryc; // til retur
        // u
        Vector3D p = p2 - *this;
        Vector3D q = p3 - *this;
        n = p^q;
        baryc.x = n.length()/areal_123;
        // v
        p = p3 - *this;
        q = p1 - *this;
        n = p^q;
        baryc.y = n.length()/areal_123;
        // w
        p = p1 - *this;
        q = p2 - *this;
        n = p^q;
        baryc.z = n.length()/areal_123;

        return baryc;
    }

};

} //namespace

#endif // VECTOR3D_H
