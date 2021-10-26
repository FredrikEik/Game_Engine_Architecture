#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "vector3d.h"

struct AABB
{
    gsl::Vector3D m_max;
    gsl::Vector3D m_min;
    void setAABB(gsl::Vector3D max, gsl::Vector3D min)
    {
        m_max = max;
        m_min = min;
    }
};

class CollisionSystem
{
public:
    CollisionSystem();
    bool AABBtoAABB(const AABB &tBox1, const AABB &tBox2);

private:

};

#endif // COLLISIONSYSTEM_H
