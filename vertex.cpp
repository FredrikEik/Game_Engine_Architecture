#include "vertex.h"
#include "qvector3d.h"
#include "qvector2d.h"


Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

    m_uv[0] = u;
    m_uv[1] = v;
}

Vertex::Vertex(QVector3D posVec, QVector3D normalVec)
{
    m_xyz[0] = posVec.x();
    m_xyz[1] = posVec.y();
    m_xyz[2] = posVec.z();

    m_normal[0] = normalVec.x();
    m_normal[1] = normalVec.y();
    m_normal[2] = normalVec.z();

    m_uv[0] = 1.f;
    m_uv[1] = 1.f;
}

Vertex::Vertex(QVector3D posVec, QVector3D normalVec, QVector2D UVvec)
{
    m_xyz[0] = posVec.x();
    m_xyz[1] = posVec.y();
    m_xyz[2] = posVec.z();

    m_normal[0] = normalVec.x();
    m_normal[1] = normalVec.y();
    m_normal[2] = normalVec.z();

    m_uv[0] = UVvec.x();
    m_uv[1] = UVvec.y();
}

QVector3D Vertex::getPosition()
{
    return QVector3D(m_xyz[0], m_xyz[1], m_xyz[2]);
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_uv[0] << ", " << v.m_uv[1] << ") ";

  return os;
}

std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;

  return is;
}
