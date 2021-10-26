#ifndef COLLISIONSHAPES_H
#define COLLISIONSHAPES_H

#include "matrix4x4.h"
#include "vertex.h"
#include "gameobject.h"
#include "components.h"
#include "vector3d.h"
#include "math.h"
#include <vector>
#include <string>

class GameObject; // C2061 fix

struct CollisionShape
{
    virtual void setPoint(float sx, float sy, float sz)=0;
    virtual void setRadius(float r)=0;
    virtual Vertex getPoint()=0;
    virtual float getRadius()=0;
};

struct CollisionSphere : CollisionShape
{
    Vertex m_cPoint;
    float m_radius;
    std::vector<Vertex> m_vertices;

    void setPoint(float sx, float sy, float sz) override
    {
        m_cPoint.set_xyz(sx, sy, sz);
    }
    void setRadius(float r) override
    {
        m_radius = r;
    }
    Vertex getPoint() override
    {
        return m_cPoint;
    }
    float getRadius() override
    {
        return m_radius;
    }
};

class CollisionShapes
{
public:
    CollisionShapes();
    void createCollisionSphere(GameObject &obj);
    gsl::Vector3D createCentroid(GameObject &obj);
    float findRadian(GameObject &obj, gsl::Vector3D centroid);
    CollisionSphere* createSphere(float fl_r, gsl::Vector3D vec_centroid);
    CollisionShape* getGeometricForm();

private:
    std::string m_cName; // Name

    CollisionShape* m_cGeometricForm; // Geometric Form
    gsl::Matrix4x4 m_cTransform; // Transform
};

#endif // COLLISIONSHAPES_H
