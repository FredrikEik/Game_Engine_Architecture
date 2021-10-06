#ifndef COLLISIONSHAPES_H
#define COLLISIONSHAPES_H

#include "matrix4x4.h"
#include "vertex.h"
#include "gameobject.h"
#include "components.h"
#include "vector3d.h"
#include <vector>
#include <string>

struct CollisionShape
{

};

struct CollisionSphere : CollisionShape
{
    Vertex m_cPoint;
    float m_radius;

    void setPoint(float sx, float sy, float sz)
    {
        m_cPoint.set_xyz(sx, sy, sz);
    }
    void setRadius(float r)
    {
        m_radius = r;
    }
};

class CollisionShapes
{
public:
    CollisionShapes();
    void createCollisionSphere(GameObject& obj);
    gsl::Vector3D createCentroid(GameObject& obj);

private:
    std::string m_cName; // Name

    std::vector<CollisionShape> m_cGeometricForm; // Geometric Form
    gsl::Matrix4x4 m_cTransform; // Transform
};

#endif // COLLISIONSHAPES_H
