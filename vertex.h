#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>

class Vertex
{

friend std::ostream& operator<< (std::ostream&, const Vertex&);

//! Overloaded ostream operator which reads all vertex data from an open textfile stream
friend std::istream& operator>> (std::istream&, Vertex&);

public:
    Vertex();
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(float x, float y, float z, float r, float g, float b, float u, float v);
    Vertex(class QVector3D posVec, class QVector3D normalVec, class QVector2D UVvec);
    QVector3D getPosition();
private:
    float m_xyz[3];
    float m_normal[3];
    float m_uv[2];
};

#endif // VERTEX_H
